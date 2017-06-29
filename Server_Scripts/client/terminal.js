var PROMPT;
var PROMPT_OUTPUT;
var IPROMPT_LEN;
var OPROMPT_LEN;
var LINE_HEIGHT;
var SPACE_LENGTH;
var PANEL_SIZE;
var WAS_OK;
var SERVER_ANSWERED;
var PANEL_UPDATE_INTERVAl;
var SATISFACTION;
var test = 0;

// TODO: make message verdicts colorful, give better names in outputBlock

window.downloadFile = function (sUrl) {

    //iOS devices do not support downloading. We have to inform user about this.
    if (/(iP)/g.test(navigator.userAgent)) {
        alert('Your device do not support files downloading. Please try again in desktop browser.');
        return false;
    }

    //If in Chrome or Safari - download via virtual link click
    if (window.downloadFile.isChrome || window.downloadFile.isSafari) {
        //Creating new link node.
        var link = document.createElement('a');
        link.href = sUrl;

        if (link.download !== undefined) {
            //Set HTML5 download attribute. This will prevent file from opening if supported.
            var fileName = sUrl.substring(sUrl.lastIndexOf('/') + 1, sUrl.length);
            link.download = fileName;
        }

        //Dispatching click event.
        if (document.createEvent) {
            var e = document.createEvent('MouseEvents');
            e.initEvent('click', true, true);
            link.dispatchEvent(e);
            return true;
        }
    }

    // Force file download (whether supported by server).
    var query = '?download';

    window.open(sUrl + query, '_self');
}
window.downloadFile.isChrome = navigator.userAgent.toLowerCase().indexOf('chrome') > -1;
window.downloadFile.isSafari = navigator.userAgent.toLowerCase().indexOf('safari') > -1;

function Normalized(string) {
	
	var index = string.indexOf('#');
	if(index != -1) {
		var sym = '';
		for(var i = 0; i < 4; ++i) {
			sym += string.charAt(index + i);
		}
		var e = sym;
		if(sym == "#col") e = ":";
		//else if ... other symbols
		return Normalized(string.replace(sym, e));
		
	}
	return string;
}
function RShandler(rsp_string) {
	
	//alert(rsp_string);
	var rs_parts = rsp_string.split(';');
	if(rs_parts.length != 3) {
		NewOutputBlock("[ERROR:]", "client:: broken RSP received.");
		return true;
	}
	else {
		if(rs_parts[1] != "void") {
			var messages = rs_parts[1].split(',');
			for(var i = 0; i < messages.length; ++i) {
				var ms = messages[i].split(':');
				if(ms.length != 3) {
					NewOutputBlock("[WARNING:]", "client:: RSP message block is broken.");
				}
				else {
					for(var i = 0; i < 3; ++i) {
						ms[i] = Normalized(ms[i]);
					}
					NewOutputBlock("[" + ms[0] + "]", ms[1] + ":: " + ms[2]);
				}
			}
		}
		if(rs_parts[2] != "void") {
			var data = rs_parts[2].split(',');
			for(var i = 0; i < data.length; ++i) {
				var datum = data[i].split(':');
				if(datum.length != 2) {
					NewOutputBlock("[WARNING:]", "client:: RSP data block is broken.");
				}
				else {
					if(datum[0] == 'FILE') {
						window.downloadFile(Normalized(datum[1]));
					}
					else if(datum[0] == "IMAGE") {
						NewOutputImage(Normalized(datum[1]));
					}
					else if(datum[0] == "TEXT") {
						NewOutputBlock("", Normalized(datum[1]));
					}
					else {
						NewOutputBlock("[WARNING:]", "client:: unsupported RSP data type.");
					}
				}
			}
		}
	}
	return rs_parts[0] == "DIE";
}
function HTTPStatusMessage(status) {	
	switch(status) {
		case	  0	:	return "client:: cross domain request.";
		case	100	:	return "100: Continue";
		case	101	:	return "101: Switching Protocols";
		case	102	:	return "102: Processing";
		case	200	:	return "200: OK";
		case	201	:	return "201: Created";
		case	202	:	return "202: Accepted";
		case	203	:	return "203: Non-Authoritative Information";
		case	204	:	return "204: No Content";
		case	205	:	return "205: Reset Content";
		case	206	:	return "206: Partial Content";
		case	207	:	return "207: Multi-Status";
		case	226	:	return "226: IM Used";
		case	300	:	return "300: Multiple Choices";
		case	301	:	return "301: Moved Permanently";
		case	302	:	return "302: Found or Moved Temporarily";
		case	303	:	return "303: See Other";
		case	304	:	return "304: Not Modified";
		case	305	:	return "305: Use Proxy";
		case	307	:	return "307: Temporary Redirect";
		case	400	:	return "400: Bad Request";
		case	401	:	return "401: Unauthorized";
		case	402	:	return "402: Payment Required";
		case	403	:	return "403: Forbidden";
		case	404	:	return "404: Not Found";
		case	405	:	return "405: Method Not Allowed";
		case	406	:	return "406: Not Acceptable";
		case	407	:	return "407: Proxy Authentication Required";
		case	408	:	return "408: Request Timeout";
		case	409	:	return "409: Conflict";
		case	410	:	return "410: Gone";
		case	411	:	return "411: Length Required";
		case	412	:	return "412: Precondition Failed";
		case	413	:	return "413: Request Entity Too Large";
		case	414	:	return "414: Request-URI Too Large";
		case	415	:	return "415: Unsupported Media Type";
		case	416	:	return "416: Requested Range Not Satisfiable";
		case	417	:	return "417: Expectation Failed";
		case	422	:	return "422: Unprocessable Entity";
		case	423	:	return "423: Locked";
		case	424	:	return "424: Failed Dependency";
		case	425	:	return "425: Unordered Collection";
		case	426	:	return "426: Upgrade Required";
		case	428	:	return "428: Precondition Required";
		case	429	:	return "429: Too Many Requests";
		case	431	:	return "431: Request Header Fields Too Large";
		case	434	:	return "434: Requested host unavailable";
		case	449	:	return "449: Retry With";
		case	451	:	return "451: Unavailable For Legal Reasons";
		case	500	:	return "500: Internal Server Error";
		case	501	:	return "501: Not Implemented";
		case	502	:	return "502: Bad Gateway";
		case	503	:	return "503: Service Unavailable";
		case	504	:	return "504: Gateway Timeout";
		case	505	:	return "505: HTTP Version Not Supported";
		case	506	:	return "506: Variant Also Negotiates";
		case	507	:	return "507: Insufficient Storage";
		case	508	:	return "508: Loop Detected";
		case	509	:	return "509: Bandwidth Limit Exceeded";
		case	510	:	return "510: Not Extended";
		case	511	:	return "511: Network Authentication Required";
	};
}
function fixIntegerLength(_value){
	var fixedInteger = "";
	var index = 9;
	while(_value != 0){
		fixedInteger += String.fromCharCode(_value % 10 + 48);
		_value = Math.floor(_value /= 10);
		--index;
	}
	while(index > -1){
		fixedInteger += '0';
		--index;
	}
	return fixedInteger.split("").reverse().join("");
}
function CreateReqString(){
	var str = "", ip;
	for(var index = 0; index < PANEL_SIZE; ++index){
		ip = document.getElementById("p_e_" + index).innerHTML;
		ip = ip.split('.');
		var intValue = 0;
		for(var i = 0; i < ip.length; ++i){
			intValue <<= 8;
			intValue += parseInt(ip[i]);
		}
		str += fixIntegerLength(intValue >>> 0);
	}
	//window.alert("To Server: " + str);
	return str;
}
function SendPanel(){
	
	SERVER_ANSWERED = false;
	var httpRequest = new XMLHttpRequest();
	var requestCore = CreateReqString();
	
	if (httpRequest.overrideMimeType) {
		httpRequest.overrideMimeType('text/xml');
	}
	
	if (!httpRequest) {
		SERVER_ANSWERED = true;
		return false;
	}
	httpRequest.onreadystatechange = function(){GetPanel(httpRequest);};
	var url = "server_scripts/panel.php?panel=" + requestCore;
	
	httpRequest.open('GET', url, true);
	httpRequest.send();
	return true;
}
function GetPanel(httpRequest){

	if(httpRequest.readyState == 4) {
		if(httpRequest.status == 200){
			UpdatePanel(httpRequest.responseText);
			WAS_OK = true;
		}
		else{
			if(WAS_OK){
				ShowPanelError("Server Error.");
				WAS_OK = false;
			}
		}
		SERVER_ANSWERED = true;
	}
}
function UpdatePanel(responseText){

	if(!WAS_OK){
		document.getElementById("side-panel").style.visibility = "visible";
		document.getElementById("error-panel").style.visibility = "hidden";
		WAS_OK = true;
	}
	var debug = "";
	//window.alert("From server: " + responseText);
	for(var i = 0 ; i < responseText.length; ++i){
		debug += responseText.charCodeAt(i) + ", ";
	}
	//window.alert("From server bytes: " + debug);
	var index = 0;
	/*
		deleted items
	*/
	var part_length = responseText.charCodeAt(index);
	if(part_length == 0) {
		++ index ;
		part_length = responseText.charCodeAt(index);
	}
	++ index;
	for(var i = 0; i < part_length; ++i, ++index){
	}
	/*
		present items
	*/
	part_length = responseText.charCodeAt(index);
	if(part_length == 0) {
		++ index ;
		part_length = responseText.charCodeAt(index);
	}
	++ index;
	for(var i = 0; i < part_length; ++i, ++index){
		var ip_status = document.getElementById("st_i_" + i);
		var new_status = "";
		switch(responseText.charCodeAt(index)){
			case	0	:	new_status = "green-light"; break;
			case	1	:	new_status = "orange-light"; break;
			case	2	:	new_status = "red-light"; break;
			default		:	ShowPanelError("Wrong status indicator");
		}
		var old_status = ip_status.className;
		if(old_status != new_status){
			ip_status.className = new_status;
		}
	}
	/*
		added items
	*/
	part_length = responseText.charCodeAt(index);
	if(part_length == 0) {
		++ index ;
		part_length = responseText.charCodeAt(index);
	}
	++ index;
	for(var i = 0; i < part_length; ++i, index += 4){
		var new_ip = responseText.charCodeAt(index)     + '.' +
					 responseText.charCodeAt(index + 1) + '.' +
					 responseText.charCodeAt(index + 2) + '.' +
					 responseText.charCodeAt(index + 3);
		PanelElementCreate(new_ip, "orange");
	}
}
function HandlePanel(){

	if(SERVER_ANSWERED){
		if(!SendPanel()){
			if(WAS_OK){
				ShowPanelError("Can\'t make request.");
				WAS_OK = false;
			}
		}
	}
}
function ShowPanelError(error){
	var panel = document.getElementById("side-panel");
	panel.style.visibility = "hidden";
	
	var error_panel = document.getElementById("error-panel");
	error_panel.style.visibility = "visible";
	
	error_panel.innerHTML = error;
}
function PanelElementCreate(ip, status_color){

	var panel = document.getElementById("side-panel");
	var panel_element = document.createElement("div");
	var status_i = document.createElement("div");

	status_i.id = "st_i_" + PANEL_SIZE;
	status_i.className = status_color + "-light";
	status_i.style.top = (panel.offsetHeight - 1) + "px";
	
	panel.appendChild(status_i);
	
	panel_element.id = "p_e_" + PANEL_SIZE;
	panel_element.className = "panel-element";
	panel_element.innerHTML = ip;
	panel_element.style.top = (panel.offsetHeight - 1) + "px";
	panel_element.style.width = panel.offsetWidth - status_i.offsetWidth - SPACE_LENGTH - 2 + "px";
	panel_element.style.left = 16 + SPACE_LENGTH + "px";
	
	panel.appendChild(panel_element);
	panel.style.height = (panel.offsetHeight /*with border*/ - 2 + status_i.offsetHeight + 1 /*space*/) + "px"; // without border
	PANEL_SIZE ++;
}
function PanelElementDelete(ip){
	
	var panel_element;
	var panel = document.getElementById("side-panel");
	for(var i = 0; i < PANEL_SIZE; ++i){
		panel_element = document.getElementById("p_e_" + i);
		if(panel_element.innerHTML === ip){
			var status_i = document.getElementById("st_i_" + i);
			var panel_element_height = status_i.offsetHeight;
			
			status_i.parentNode.removeChild(status_i);
			panel_element.parentNode.removeChild(panel_element);
			panel.style.height = (panel.offsetHeight - 2 - panel_element_height - 1) + "px";

			for(var j = i + 1; j < PANEL_SIZE; ++j){
				panel_element = document.getElementById("p_e_" + j);
				status_i = document.getElementById("st_i_" + j);
				
				panel_element.id = "p_e_" + (j-1);
				status_i.id = "st_i_" + (j-1);
				panel_element.style.top = (panel_element.offsetTop - panel_element_height - 1) + "px";
				status_i.style.top = (status_i.offsetTop - panel_element_height - 1) + "px";
			}
			PANEL_SIZE --;
			i --;
		}
	}
}
function PanelElementChange(ip1, ip2, status2){
	
	var panel_element, status_i;
	for(var i = 0; i < PANEL_SIZE; ++i){
		
		panel_element = document.getElementById("p_e_" + i);
		if(panel_element.innerHTML === ip1){
			panel_element.innerHTML = ip2;
			status_i = document.getElementById("st_i_" + i);
			status_i.className = status2 + "-light";
		}
	}
}
function __window_resize(){
	var input = document.getElementById("input");
	var terminal = document.getElementById("terminal");
	var iLine_new_width = (terminal.offsetWidth - IPROMPT_LEN - SPACE_LENGTH - 3) + "px";
	var oLine_new_width = (terminal.offsetWidth - OPROMPT_LEN - SPACE_LENGTH - 5) + "px";
	input.style.width = iLine_new_width;
	
	if(PANEL_SIZE > 0){
	
		var panel = document.getElementById("side-panel");
		var status = document.getElementById("st_i_0");
		var pe_new_width = panel.offsetWidth - status.offsetWidth - SPACE_LENGTH - 2 + "px";
		for(var i = 0; i < PANEL_SIZE; ++i) {
			var pe = document.getElementById("p_e_" + i);
			pe.style.width = pe_new_width;
		}
	}
	
	var old_coms = document.querySelectorAll(".input-line-old");
	for(var i = 0; i < old_coms.length; ++i){
		old_coms.item(i).style.width = iLine_new_width;
	}
	
	var old_outputs = document.querySelectorAll(".output-block");
	for(var i = 0; i < old_outputs.length; ++i){
		old_outputs.item(i).style.width = oLine_new_width;
	}
}
function FirstLaunchInit(){

	PANEL_SIZE = 0;
	var terminal = document.getElementById("terminal");
	LINE_HEIGHT = terminal.offsetHeight;
	SPACE_LENGTH = 5;
	PANEL_UPDATE_INTERVAl = 5;
	WAS_OK = true;
	SERVER_ANSWERED = true;
	PROMPT = "$ >>>";
	PROMPT_OUTPUT = "[WARNING:] ";
	IPROMPT_LEN = 50;
	OPROMPT_LEN = 95;
	SATISFACTION = false;
	NewInputLine();
	setInterval(HandlePanel, PANEL_UPDATE_INTERVAl*1000);
	
	var request = "server_scripts/clear_session.php";
	var httpRequest = new XMLHttpRequest();

	if (!httpRequest) {
		// some error
		window.alert("Previous session has not been cleared.");
	}
	httpRequest.open('HEAD', request, true);
	httpRequest.send();
}
function DeleteLine() {
	
	var old_input = document.getElementById("input");
	if(old_input != null) {
		var old_prompt = document.getElementById("prompt");
		var terminal = document.getElementById("terminal");
		var new_height = terminal.offsetHeight + old_input.offsetHeight - 2;
		terminal.style.height = new_height + "px";
		if(old_input.className == "input-line") old_input.className = "input-line-old";
		old_input.removeAttribute('id');
		old_input.removeAttribute('onkeydown');
		old_prompt.removeAttribute('id');
		old_input.readOnly = true;
		old_input.autofocus = false;
		terminal.appendChild(old_input);
		terminal.appendChild(old_prompt);
	}
}
function NewInputLine(){
	
	DeleteLine();
	var terminal = document.getElementById("terminal");
	var height = terminal.offsetHeight;
	
	var new_prompt = document.createElement("div");
	new_prompt.id = "prompt";
	new_prompt.className = "prompt-line";
	new_prompt.innerHTML = PROMPT;
	new_prompt.style.top = height + "px";
	new_prompt.style.width = IPROMPT_LEN + "px";
	terminal.appendChild(new_prompt);

	var new_input = document.createElement("input");
	new_input.id = "input";
	new_input.onkeydown = function(event){
		switch(event.keyCode){
			case	0x0d	:	CommandHandler(); break;
		};
	}
	new_input.className = "input-line";
	new_input.style.top = height - 1 + "px";
	new_input.style.width = (terminal.offsetWidth - new_prompt.offsetWidth - SPACE_LENGTH - 1) + "px";
	new_input.style.left = new_prompt.offsetWidth + SPACE_LENGTH + "px";
	new_input.autofocus = true;
	
	terminal.appendChild(new_input);
	new_input.focus();
}
function NewOutputBlock(prompt, string) {
	
	DeleteLine();
	var terminal = document.getElementById("terminal");
	var height = terminal.offsetHeight;
	
	var new_prompt = document.createElement("div");
	new_prompt.id = "prompt";
	new_prompt.className = "prompt-line";
	new_prompt.innerHTML = prompt;
	new_prompt.style.top = (height - 2) + "px";
	new_prompt.style.width = OPROMPT_LEN + "px";
	terminal.appendChild(new_prompt);

	var new_output = document.createElement("div");
	new_output.id = "input";
	new_output.className = "output-block";
	new_output.style.top = (height - 2) + "px";
	new_output.style.width = (terminal.offsetWidth - new_prompt.offsetWidth - SPACE_LENGTH - 3) + "px";
	new_output.style.left = new_prompt.offsetWidth + SPACE_LENGTH + "px";
	new_output.innerHTML = string;
	
	terminal.appendChild(new_output);
	new_output.focus();
}
function NewOutputImage(url) {
	
}
function CommandHandler(){
	CommandParsing(document.getElementById("input").value);
	NewInputLine();
}
function CommandParsing(command_string) {
	
	if(command_string[0] == '$') {
		var command_name = "";
		for(var i = 0; i < command_string.length; ++i) {
			if(command_string[i] == ' ') break;
			command_name += command_string[i];
		}
		
		if(command_name == "$echo") ECHO(command_string);
	 // else if(command_name == .....
	    else {
			NewOutputBlock("[ERROR:]", "client:: no such command");
		}
	}
	else {
		StartExecution(command_string);
	}
}
function StartExecution(command_string) {
	
	var array_string = command_string.split(" ");
	var command_name = array_string[0];
	var args_string = "";
	for(var i = 1; i < array_string.length; ++i) {
		args_string += array_string[i];
		if(i == array_string.length - 1) break;
		args_string += ',';
	}
	window.alert(command_name + " -> " + args_string);
	var request = "server_scripts/commands.php?command=" + command_name + "&arguments=" + encodeURIComponent(args_string);
	var httpRequest = new XMLHttpRequest();
	
	if (httpRequest.overrideMimeType) {
		httpRequest.overrideMimeType('text/xml');
	}
	
	if (!httpRequest) {
		// some error
		alert("error");
	}
	httpRequest.onreadystatechange = function(){
		FirstLevelHandler(httpRequest);
	};
	
	httpRequest.open('GET', request, true);
	httpRequest.send();
	
	//while(!SATISFACTION){
		
		//window.alert("waiting!");
	//}
	SATISFACTION = false;
}
function FirstLevelHandler(httpRequest) {
		//alert("Changing1: " + httpRequest.readyState);
		if(httpRequest.readyState == 4) {
			if(httpRequest.status == 200){
				//alert("Changing2");
				NewOutputBlock("[INFO:]", HTTPStatusMessage(httpRequest.status));
				SATISFACTION = RShandler(httpRequest.responseText);
				alert(httpRequest.responseText);
				if(!SATISFACTION) {
					//alert("Changing3");
					// run thread by sending request to get infromation until DIE
					// launche ContinueExecution(..) with handler SecondLevelHandler(.) in some interval
				}
			}
			else {
				NewOutputBlock("[ERROR:]", HTTPStatusMessage(httpRequest.status));
				SATISFACTION = true;
			}
		}
}
function SecondLevelHandler(httpRequest) {
}
// TERMINAL COMMANDS' FUNCTIONS
function ECHO(command) {
	
	var args = command.split(" ");
	for(var i = 1; i < args.length; ++i) {
		NewOutputBlock("", args[i]);
	}
}