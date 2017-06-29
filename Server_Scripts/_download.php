<?php
	include_once('rqp_rsp.php');

	function rand_name($length) {
		$name = '';
		srand();
		for($i = 0; $i < $length; ++$i) {
			$case = rand(0, 2);
			if($case == 0) {
				$sym = chr(rand(97, 122));
			}
			else if ($case == 1) {
				$sym = chr(rand(65, 90));
			}
			else if ($case == 2) {
				$sym = chr(rand(48, 57));
			}
			$name .= $sym;
		}
		return $name;
	}
	function create_archive($arch_name, $file_path) {
		
		$zip = new ZipArchive; 
		if ($zip->open($arch_name, ZipArchive::CREATE) === TRUE) { 
			if(is_dir($file_path)) {
				$dirs = scandir($file_path); 
				for($i = 2; $i < count($dirs); ++$i) { 
					
					if(is_dir($file_path . $dirs[$i])) {
						
						$zip->addEmptyDir($dirs[$i]);
						$sub_dirs = scandir($file_path . $dirs[$i]);
						for($j = 2; $j < count($sub_dirs); ++$j) {
							$dirs[] = $dirs[$i] . '/' . $sub_dirs[$j];
						}
					}
					else {
						$zip->addFile(realpath($file_path . $dirs[$i]), $dirs[$i]); 
					}
				}
			}
			else {
				echo  $file_path . ' ------ ';
				$zip->addFile(realpath($file_path), basename($file_path)); 
			}
			$zip->close();
			echo 'Archiving is sucessful!'; 
		} 
		else { 
			echo 'Error, can\'t create a zip file!'; 
		}
	}
	
	function _DOWNLOAD($arg) {
		
		if($arg[0] == '/') $path = '/archive' . $arg;
		else {
			$fs_props = fopen('../buffer/filesystem.props', 'r');
			$cur_dir = fgets($fs_props);
			$path = $cur_dir . '/' . $arg;
			fclose($fs_props);
		}
		if(!file_exists('..' . $path)) {
			echo toRSPstring('DIE', array('ERROR', 'Invalid file/directory path.'),  array());
			return;
		}
		
		if(is_dir('..' . $path) && $path[strlen($path) - 1] != '/') $path .= '/';
		
		$zip_path = 'temporary/' . rand_name(10) . '.zip';
		create_archive('../' . $zip_path, '..' . $path);
		echo toRSPstring('DIE', array(),  array('FILE', $zip_path));
	}

?>