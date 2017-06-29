<?php
	include_once('rqp_rsp.php');

	function _ECHO($args) {
			$args_array = explode(',', $args);
			$output = '';
			for($i = 0; $i < count($args_array); ++$i) {
				$output .= $args_array[$i];
				if($i == count($args_array) - 1) break;
				$output .= '<br>';
			}
			echo toRSPstring('DIE', array(), array('TEXT', $output));
	}
?>