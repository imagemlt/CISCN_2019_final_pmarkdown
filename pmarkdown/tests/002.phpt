--TEST--
pmarkdown_test1() Basic test
--SKIPIF--
<?php
if (!extension_loaded('pmarkdown')) {
	echo 'skip';
}
?>
--FILE--
<?php
$ret = pmarkdown_test1();

var_dump($ret);
?>
--EXPECT--
The extension pmarkdown is loaded and working!
NULL
