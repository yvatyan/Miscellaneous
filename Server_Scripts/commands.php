<?php
	include_once('_echo.php');
	include_once('_download.php');
	include_once('_ls.php');
	include_once('_cd.php');
	include_once('rqp_rsp.php');

	$command = $_GET['command'];
	$args = $_GET['arguments'];

	if($command[0] == '@') {
		     if($command == '@echo')     _ECHO($args);
		else if($command == '@download') _DOWNLOAD($args);
		else if($command == '@ls')		 _LS($args);
		else if($command == '@cd')		 _CD($args);
		// else if($command == "@......
		else {
			echo toRSPstring("DIE", array("ERROR", "no such command"), array()); 
		}
	}
	else {
		// send command to station
		echo 'Else';
	}
	
?>