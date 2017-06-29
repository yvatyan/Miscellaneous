<?php

	$fs_props = fopen('../buffer/filesystem.props', 'w');
	fputs($fs_props, '/archive');
	$path = '../temporary/';
	$files = scandir($path);
	for($i = 2; $i != count($files); ++$i) {
		unlink($path . $files[$i]);
	}
?>