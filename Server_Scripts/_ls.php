<?php

	include_once('rqp_rsp.php');
	
	function spaces($count) {
		$str = '';
		for($i = 0; $i < $count; ++$i) {
			$str .= ' ';
		}
		return $str;
	}
	
	function _LS($arg) {
		
		if(strlen($arg) == 0) {
			$fs_props = fopen('../buffer/filesystem.props', 'r');
			$cur_dir = fgets($fs_props);
			$path = '..' . $cur_dir . '/' . $arg;
			fclose($fs_props);			
		}
		else {
			if($arg[0] == '/') $path = '../archive' . $arg;
			else {
				$fs_props = fopen('../buffer/filesystem.props', 'r');
				$cur_dir = fgets($fs_props);
				$path = '..' . $cur_dir . '/' . $arg;
				fclose($fs_props);
			}
		}
		
		if(!file_exists($path)) {
			echo toRSPstring('DIE', array('ERROR', 'Invalid file/directory path.'),  array());
			return;
		}
		if(!is_dir($path)) {
			echo toRSPstring('DIE', array('ERROR', 'Can not list file.'),  array());
		}
		else {
			$files = scandir($path);
			$output = '<pre>';
			for($i = 0; $i < count($files); ++$i) {
				
				$stat = stat($path . '/' . $files[$i]);
				if(is_dir($path . '/' . $files[$i])) {
					$output .= 'DIR' . spaces(4) . date('d M Y D H\#\c\o\li', $stat['mtime']) . spaces(4 + 10 + 4) . $files[$i] . '<br>';
				}
				else {
					$output .= spaces(7) . date('d M Y D H\#\c\o\li', $stat['mtime']) . spaces(4) . $stat['size'] . spaces(10 - strlen('' . ($stat['size'])) + 4) . $files[$i] . '<br>';
				}
			}
			echo toRSPstring('DIE', array(),  array('TEXT', $output . '</pre>'));
		}
	}
?>