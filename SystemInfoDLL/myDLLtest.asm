format PE GUI 4.0
entry main
include 'win32a.inc'

WIN_NAME_LENGTH = 28

section '.data' readable writeable
	windows db 'Windows 2000', 16 dup (0), \
		   'Windows XP', 18 dup (0), \
		   'Windows XP x64 Professional', 0, \
		   'Windows Server 2003', 9 dup (0), \
		   'Windows Home Server', 9 dup (0), \
		   'Windows Server 2003 R2', 6 dup (0), \
		   'Windows Server 2008', 9 dup (0), \
		   'Windows Vista', 15 dup (0), \
		   'Windows Server 2008 R2', 6 dup (0), \
		   'Windows 7', 19 dup (0), \
		   'Windows Server 2012', 9 dup (0), \
		   'Windows 8', 19 dup (0), \
		   'Windows Server 2012 R2', 6 dup (0), \
		   'Windows 8.1', 17 dup (0), \
		   'Windows Server 2016', 9 dup (0), \
		   'Windows 10', 0
	hex    db '0123456789ABCDEF'
	number db '0x00000000',0
     user_name du 'User name',0
     comp_name du 'Computer name',0
       win_ver db 'Windows version',0
      ram_size db 'RAM (Hexadecimal)',0
     cpu_brand db 'CPU',0


section '.text' code readable executable

	proc main
	     invoke GetUserName
	     add    eax, 4
	     invoke MessageBoxW, HWND_DESKTOP, eax, user_name, MB_OK+MB_ICONINFORMATION
	     invoke GetPCName
	     add    eax, 4
	     invoke MessageBoxW, HWND_DESKTOP, eax, comp_name, MB_OK+MB_ICONINFORMATION
	     invoke Windows
	     call   ShowWindowsVersion
	     invoke GetMemory
	     call   ShowInteger32Eax
	     invoke GetCPUName
	     invoke MessageBoxA, HWND_DESKTOP, eax, cpu_brand, MB_OK+MB_ICONINFORMATION
	     invoke ExitProcess, 0
	endp

	proc ShowInteger32Eax
	     mov    ebx, hex
	     mov    edx, eax
	     mov    ecx, 8
	     xor    eax, eax
	     mov    esi, number
	divide:
	     mov    al,  dl
	     and    al,  0x0F
	     xlatb
	     mov    byte [esi + ecx + 1], al
	     shr    edx, 4
	     loop   divide
	     mov    eax, number
	     invoke MessageBoxA, HWND_DESKTOP, eax, ram_size, MB_OK+MB_ICONINFORMATION
	     ret
	endp

	proc ShowWindowsVersion
	     push   ebx
	     push   eax
	     push   esi
	     push   edx

	     mov    ebx, eax
	     and    ebx, 0x000000FF
	     cmp    ebx, 5
	     je     windows_old
	     cmp    ebx, 6
	     je     windows_new
	     ; windows_future (2015, only windows_10_gen)
	     mov    ebx, eax
	     and    ebx, 0x00FF0000
	     shr    ebx, 16
	     test   ebx, ebx
	     jnz    windows_10
	     mov    eax, 14
	     jmp    finalize
	windows_old:
	     mov    ebx, eax
	     and    ebx, 0x0000FF00
	     shr    ebx, 8
	     cmp    ebx, 0
	     je     windows_2000
	     cmp    ebx, 1
	     je     windows_xp

	     mov    ebx, eax
	     and    ebx, 0x00FF0000
	     shr    ebx, 16
	     cmp    ebx, 0
	     je     windows_xp_64
	     cmp    ebx, 1
	     je     windows_2003
	     cmp    ebx, 2
	     je     windows_home
	     ;windows_2003_R2
	     mov    eax, 5
	     jmp    finalize

	windows_new:
	     mov    ebx, eax
	     and    ebx, 0x0000FF00
	     shr    ebx, 8
	     cmp    ebx, 0
	     je     windows_vista_gen
	     cmp    ebx, 1
	     je     windows_7_gen
	     cmp    ebx, 2
	     je     windows_8_gen
	     ; windows_8_1_gen
	     mov    ebx, eax
	     and    ebx, 0x00FF0000
	     shr    ebx, 16
	     test   ebx, ebx
	     jnz    windows_8_1
	     mov    eax, 12
	     jmp    finalize

	windows_vista_gen:
	     mov    ebx, eax
	     and    ebx, 0x00FF0000
	     shr    ebx, 16
	     test   ebx, ebx
	     jnz    windows_vista
	     mov    eax, 6
	     jmp    finalize

	windows_7_gen:
	     mov    ebx, eax
	     and    ebx, 0x00FF0000
	     shr    ebx, 16
	     test   ebx, ebx
	     jnz    windows_7
	     mov    eax, 8
	     jmp    finalize
	windows_8_gen:
	     mov    ebx, eax
	     and    ebx, 0x00FF0000
	     shr    ebx, 16
	     test   ebx, ebx
	     jnz    windows_8
	     mov    eax, 10
	     jmp    finalize

	windows_2000:
	     mov    eax, 0
	     jmp    finalize
	windows_xp:
	     mov    eax, 1
	     jmp    finalize
	windows_xp_64:
	     mov    eax, 2
	     jmp    finalize
	windows_2003:
	     mov    eax, 3
	     jmp    finalize
	windows_home:
	     mov    eax, 4
	     jmp    finalize
	windows_vista:
	     mov    eax, 7
	     jmp    finalize
	windows_7:
	     mov    eax, 9
	     jmp    finalize
	windows_8:
	     mov    eax, 11
	     jmp    finalize
	windows_8_1:
	     mov    eax, 13
	     jmp    finalize
	windows_10:
	     mov    eax, 15

	finalize:
	     mov    ebx, WIN_NAME_LENGTH
	     mul    ebx
	     mov    esi, windows
	     add    esi, eax
	     invoke MessageBoxA, HWND_DESKTOP, esi, win_ver, MB_OK+MB_ICONINFORMATION

	     pop    edx
	     pop    esi
	     pop    eax
	     pop    ebx
	     ret
	endp

section '.idata' import data readable writeable

     library kernel,  'KERNEL32.DLL',\
	     sysinfo, 'SYSINFO32.DLL',\
	     user,    'USER32.DLL'

     import  kernel,\
	     ExitProcess, 'ExitProcess'

     import  user,\
	     MessageBoxW, 'MessageBoxW',\
	     MessageBoxA, 'MessageBoxA'

     import  sysinfo,\
	     GetUserName, '__user',\
	     GetPCName,   '__computer',\
	     Windows,	  '__windows',\
	     GetMemory,   '__memory',\
	     GetCPUName,  '__processor'

