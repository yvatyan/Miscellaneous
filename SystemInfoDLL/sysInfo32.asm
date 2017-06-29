format PE console DLL
entry DllEntry
include 'win32a.inc'

if ~ defined _UNICODE
      OSVERSION_INFO_EX_SIZE = 284
else
      OSVERSION_INFO_EX_SIZE = 256
end if
      SM_SERVERR2 = 89
      VER_SUITE_WH_SERVER = 0x8000
      VER_NT_WORKSTATION  = 0x01


section '.data' data readable writeable
	PhysicalMemory	db 'GetPhysicallyInstalledSystemMemory', 0
	KERNEL32		db 'kernel32.dll', 0
	Win8Point1OrGr	db 'IsWindows8Point1OrGreater', 0
	Win10OrGreater	db 'IsWindows10OrGreater', 0

section '.text' code readable executable

	proc DllEntry hinstDll, fdwReason, lpvReserved
	     mov	eax, TRUE
	     ret
	endp

	proc __user
	     invoke	LocalAlloc, LMEM_FIXED, 256*2+4
	     mov	esi, eax
	     invoke	LocalAlloc, LMEM_FIXED, 4
	     mov	ebx, eax
	     mov	dword [ebx], 256
	     mov	[esi], ebx
	     add	esi, 4
	     invoke	GetUserName, esi, ebx
	     sub	esi, 4
	     invoke	LocalFree, ebx
	     mov	eax, esi
	     ret
	endp

	proc __computer
	     invoke	LocalAlloc, LMEM_FIXED, 256*2+4
	     mov	esi, eax
	     invoke	LocalAlloc, LMEM_FIXED, 4
	     mov	ebx, eax
	     mov	dword [ebx], 256
	     mov	[esi], ebx
	     add	esi, 4
	     invoke	GetCompName, esi, ebx
	     sub	esi, 4
	     invoke	LocalFree, ebx
	     mov	eax, esi
	     ret
	endp

	proc __windows
	     invoke	LocalAlloc, LMEM_FIXED, OSVERSION_INFO_EX_SIZE
	     mov	esi, eax

	     mov	dword [esi], OSVERSION_INFO_EX_SIZE
	     invoke	GetVersionEx, esi

	     xor	ebx, ebx
	     mov	edx, dword [esi+4]
	     cmp	edx, 5
	     je 	windows_old
	     cmp	byte [esi + OSVERSION_INFO_EX_SIZE - 2], VER_NT_WORKSTATION
	     jne	finalize
	     or 	ebx, 1
	     jmp	finalize
	windows_old:
	     mov	edx, dword [esi+8]
	     cmp	edx, 2				; windows 2000 or windows xp
	     jb 	finalize
	     test	word [esi + OSVERSION_INFO_EX_SIZE - 4], VER_SUITE_WH_SERVER
	     jnz	windows_home_server
	     invoke	GetSysMetrics, SM_SERVERR2
	     test	eax, eax
	     jnz	windows_server_2003_R2
	     cmp	byte [esi + OSVERSION_INFO_EX_SIZE - 2], VER_NT_WORKSTATION
	     jne	windows_server_2003
	     jmp	finalize
	windows_server_2003:
	     or 	ebx, 1				; windows server 2003
	     jmp	finalize
	windows_home_server:
	     or 	ebx, 2				; windows home server
	     jmp	finalize
	windows_server_2003_R2:
	     or 	ebx, 3				; windows server 2003 R2
	finalize:
	     shl	ebx, 8
	     or 	ebx, dword [esi+8]
	     shl	ebx, 8
	     or 	ebx, dword [esi+4]
	     invoke	LocalFree, esi
	     mov	eax, ebx
	     ret
	endp

	proc _win7above
	     push	ebx
	     push	ecx
	     invoke	GetVersion
	     mov	ebx, eax
	     mov	ecx, eax
	     xor	eax, eax
	     and	ebx, 0x000000FF
	     cmp	ebx, 6
	     jb 	finalize2
	     and	ecx, 0x0000FF00
	     shr	ecx, 8
	     cmp	ecx, 1
	     jb 	finalize2
	     not	eax
	finalize2:
	     pop	ecx
	     pop	ebx
	     ret
	endp

	proc  __memory
	     call	_win7above
	     test	eax, eax
	     jnz	new_version
	     invoke	LocalAlloc, LMEM_FIXED, 64
	     mov	esi, eax
	     mov	dword [esi], 64
	     invoke	GlobalMemory, esi		; memory in bytes
	     mov	ecx, dword [esi+8]
	     mov	edx, dword [esi+12]
	     shrd	ecx, edx, 10			; kilobytes in ecx, edx ignored
	     jmp	finalize3
	new_version:
	     invoke	GetModuleHandle, KERNEL32
	     invoke	GetProcAddress, eax, PhysicalMemory
	     mov	edi, eax
	     invoke	LocalAlloc, LMEM_FIXED, 8
	     mov	esi, eax
	     ;invoke	PhysicalMemory, esi
	     push	esi
	     call	edi
	     mov	ecx, dword [esi]
	finalize3:
	     push	ecx
	     invoke	LocalFree, esi
	     pop	ecx
	     mov	eax, ecx
	     ret
	endp

	proc __processor
	    invoke	LocalAlloc, LMEM_FIXED, 48
	    mov 	esi, eax
	    mov 	eax, 0x80000002
	    cpuid
	    mov 	dword [esi],	eax
	    mov 	dword [esi+4],	ebx
	    mov 	dword [esi+8],	ecx
	    mov 	dword [esi+12], edx
	    mov 	eax, 0x80000003
	    cpuid
	    mov 	dword [esi+16], eax
	    mov 	dword [esi+20], ebx
	    mov 	dword [esi+24], ecx
	    mov 	dword [esi+28], edx
	    mov 	eax, 0x80000004
	    cpuid
	    mov 	dword [esi+32], eax
	    mov 	dword [esi+36], ebx
	    mov 	dword [esi+40], ecx
	    mov 	dword [esi+44], edx
	    mov 	eax, esi
	    ret
	endp

section '.idata' import data readable writeable

	library kernel, 	'KERNEL32.DLL',\
		advapi, 	    'ADVAPI32.DLL',\
		user,		    'USER32.DLL'

	import	kernel,\
		LocalFree,	    'LocalFree',\
		LocalAlloc,	'LocalAlloc',\
		GetCompName,	'GetComputerNameW',\
		GetVersionEx,	'GetVersionExW',\
		GetVersion,	'GetVersion',\
		GlobalMemory,	'GlobalMemoryStatusEx',\
		GetProcAddress, 'GetProcAddress',\
		GetModuleHandle,'GetModuleHandleA'

	import	advapi,\
		GetUserName,	'GetUserNameW'

	import	user,\
		GetSysMetrics,	'GetSystemMetrics'


section '.edata' export data readable  

	export 'SYSINFO32.DLL',\
	       __user,		'__user',\
	       __computer,	'__computer',\
	       __windows,	'__windows',\
	       __memory,	'__memory',\
	       __processor,	'__processor'


section '.reloc' fixups data readable discardable
