<?php

	function toRSPstring($c_verdict, $message_array, $data_array) {
			
		$response = $c_verdict . ';';
		if(count($message_array) > 0) {
			for($i = 0; $i < count($message_array); $i += 2) {
				$response .= $message_array[$i] . ':server:' . $message_array[$i+1];
				if($i == count($message_array) - 2) break;
				$response .= ',';
			}
		}
		else {
			$response .= 'void';
		}
		$response .= ';';
		if(count($data_array) > 0) {
			for($i = 0; $i < count($data_array); $i += 2) {
				$response .= $data_array[$i] . ':' . $data_array[$i+1];
				if($i == count($data_array) - 2) break;
				$response .= ',';
			} 
		}
		else {
			$response .= 'void';
		}
		return $response;
	}
?>