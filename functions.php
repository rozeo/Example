<?php
	function get_urlValue( $name ){
		return ( isset( $_GET[$name]))?$_GET[$name]:-1;
	}

	// Sieve of Eratosthenes
	function ElatSieve( $MAX,$op ){
		if( $MAX < 0 ) return print('Error.<br>set "max" Value smaller than 0,or not value set');
		for( $p = 0;$p <= $MAX;$p++ ){
			if( $p % 2 == 0 ) $flg[$p] = false;
			else              $flg[$p] = true;
		}
		$flg[1] = false;
		$flg[2] = true; 
		$cnt = 3;
		while( $cnt < sqrt($MAX)){
			if( !($flg[$cnt])){
				$cnt += 2;
				continue;
			}
			$n = $cnt+2;
			while( $n <= $MAX ){
				if( $n % $cnt == 0 ){
					$flg[$n] = false;
				} 
				$n += 2;
			}
			$cnt += 2;
		}
		if( $op > 0 ){
			$c = 0;
			for( $p = 0;$p < $MAX;$p++ ){
				if( $flg[$p] ) $c++;
				if( $c == $op ){
					echo $op,' th prime : ',$p;
					return;
				}
			}
			echo $op,' th prime was not a ',$MAX,' or less<br>';
		}else{
			$c = 0;
			for( $p = 0;$p < $MAX;$p++ ) if( $flg[$p] ) $c++;//echo $p,' ';
			echo $c;
		}
	}
?>
