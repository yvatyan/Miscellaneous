<?php

	include_once('rqp_rsp.php');
	
	function _CD($arg) {
		
		if(strlen($arg) == 0) {
			echo toRSPstring('DIE', array('WARNING', 'Directory remains the same.'),  array());
			return;
		}
		else {
			if($arg[0] == '/') $path = '/archive' . $arg;
			else {
				$fs_props = fopen('../buffer/filesystem.props', 'r');
				$cur_dir = fgets($fs_props);
				$path = $cur_dir . '/' . $arg;
				fclose($fs_props);
			}
		}
		
		if(!file_exists('..' . $path)) {
			echo toRSPstring('DIE', array('ERROR', 'Invalid directory path.'),  array());
			return;
		}
		if(!is_dir('..' . $path)) {
			echo toRSPstring('DIE', array('ERROR', 'Can not change directory to file.'),  array());
		}
		else {
			$fs_props = fopen('../buffer/filesystem.props', 'w');
			fputs($fs_props, $path);
			echo toRSPstring('DIE', array('OK', 'Directory has been changed.'),  array());
		}
	}
?>