<?php
	$CUR_TIME = time();
	$ORANGE_THRESHOLD = 8; //sec
	$RED_THRESHOLD = 16; // sec
	
	class BinaryNode{
		private $right;
		private $left;
		private $ip_key;
		private $last_mod;
		private $mark;
		
		public function  __construct($ip, $value){
			$this->right = null;
			$this->left = null;
			$this->ip_key = $ip;
			$this->last_mod = $value;
			$this->mark = false;
		}
		public function getRight(){
			return $this->right;
		}
		public function getLeft(){
			return $this->left;
		}
		public function getKey(){
			return $this->ip_key;
		}
		public function getValue(){
			return $this->last_mod;
		}
		public function setRight($right_child){
			$this->right = $right_child;
		}
		public function setLeft($left_child){
			$this->left = $left_child;
		}
		public function setMarked($marked){
			$this->mark = $marked;
		}
		public function isMarked(){
			return $this->mark;
		}
	}
	class BinarySearchTree{
		
		private $root;
		
		private function insertNode($node, $subtree){
			if($node->getKey() > $subtree->getKey()){
				if($subtree->getRight() != null)
					$this->insertNode($node, $subtree->getRight());
				else
					$subtree->setRight($node);
			}
			else{
				if($subtree->getLeft() != null)
					$this->insertNode($node, $subtree->getLeft());
				else
					$subtree->setLeft($node);
			}
		}
		private function walk($key, $node){
			
			if($node === null) return null;
			
			     if($key > $node->getKey()) return $this->walk($key, $node->getRight());
			else if($key < $node->getKey()) return $this->walk($key, $node->getLeft());
			else{
				$node->SetMarked(true);
				return $node->getValue();
			}
		}
		private function getUnmarked($node, &$ips, &$ip_counter){
			
            if($node != null){
				if(!$node->isMarked()){
					$ip = $node->getKey();
					$ip = explode('.', $ip);
					$ips .= byteValToUTF8(intval($ip[0])) . 
							byteValToUTF8(intval($ip[1])) .
							byteValToUTF8(intval($ip[2])) .
							byteValToUTF8(intval($ip[3]));
					$ip_counter ++;
				}
				$this->getUnmarked($node->getLeft(), $ips, $ip_counter);
				$this->getUnmarked($node->getRight(), $ips, $ip_counter);
            }
		}
		
		public function __construct(){
			$this->root = null;
		}
		public function IsEmpty(){
			return $this->root === null;
		}
		public function Insert($key, $value){	// $key is ip, $value is modified time

			$node = new BinaryNode($key, $value);
			if ($this->IsEmpty()) {
				$this->root = $node;
			}
			else {
				$this->insertNode($node, $this->root);
			}
		}
		public function Search($ip){
			return $this->walk($ip, $this->root);
		}
		public function GetUnMarkedIps(&$ip_counter){
			$ips = '';
			$this->getUnmarked($this->root, $ips, $ip_counter);
			return $ips;
		}
	}
	function byteValToUTF8($byte){
               return $byte > 127 ? chr(192 |  ($byte >> 6)) . chr(128 | ($byte & 63)) : chr($byte);
    }
	function byteValToUTF8parts($byte, &$high, &$low){
		if($byte < 128) {
			$high = chr(0);
			$low = chr($byte);
		}
		else {
			$high = chr(192 |  ($byte >> 6));
			$low = chr(128 | ($byte & 63));
		}
	}
	function convert10ByteIpFromString($ip_string, $offset){
		$ip = '';

		$i = 0;
		$ip_integer = 0;
		for($j = $offset; $j < 10 + $offset; ++$j){
			$ip_integer *= 10;
			$ip_integer += ord($ip_string[$j]) - 48;
		}
		$ip = ($ip_integer >> 24) . '.' .
			  (($ip_integer >> 16) & 0x000000FF) . '.' .
			  (($ip_integer >> 8) & 0x000000FF) . '.' .
			  ($ip_integer & 0x000000FF) . '.ip';
		
		return $ip;
	}
	
	$panel = $_GET['panel'];
	$panel_byte_size = strlen($panel);
	$ip_files = scandir('../buffer/pings/');

	//$panel_size = strlen($panel);
	$past_part = '**';		$past_part_quantity = 0;
	$present_part = '**';
	$future_part = '**';	$future_part_quantity = 0;
	$ip_list = new BinarySearchTree;
	
	for($i = 2; $i < count($ip_files); ++$i){
		$ip_list->Insert($ip_files[$i], filemtime('../buffer/pings/' . $ip_files[$i]));
	}
	
	for($i = 0; $i < $panel_byte_size; $i += 10) {

		$ip = convert10ByteIpFromString($panel, $i);
		//echo $ip . ' --- ';
		$last_mod = $ip_list->Search($ip);
		if($last_mod === null) {
			$ip = explode('.', $ip);
			$past_part .= byteValToUTF8(intval($ip[0])) . 
						  byteValToUTF8(intval($ip[1])) .
						  byteValToUTF8(intval($ip[2])) .
						  byteValToUTF8(intval($ip[3])) ;
			$past_part_quantity ++;
		}
		else {		
			$diff = $CUR_TIME - $last_mod;
			
			if($diff < $ORANGE_THRESHOLD){											// active
				$present_part .= chr(0);
			}
			else if($diff > $ORANGE_TRESHOLD && $diff < $RED_THRESHOLD){ 		// passive
				$present_part .= chr(1);
			}
			else if($diff > $RED_TRESHOLD){									// inactive
				$present_part .= chr(2);
			}
		}
	}
	
	$future_part .= $ip_list->GetUnMarkedIps($future_part_quantity);
	
	byteValToUTF8parts($past_part_quantity, $high, $low);
	$past_part[0] = $high; $past_part[1] = $low;
	
	byteValToUTF8parts(strlen($present_part) - 2, $high, $low);
	$present_part[0] = $high; $present_part[1] = $low;
	
	byteValToUTF8parts($future_part_quantity, $high, $low);
	$future_part[0] = $high; $future_part[1] = $low;

	echo $past_part;
	echo $present_part;
	echo $future_part;
	EX:
?>