--TEST--
pmarkdown_test2() Basic test
--SKIPIF--
<?php
if (!extension_loaded('pmarkdown')) {
	echo 'skip';
}
?>
--FILE--
<?php
var_dump(pmarkdown_test2());
var_dump(pmarkdown_test2('PHP'));
?>
--EXPECT--
string(11) "Hello World"
string(9) "Hello PHP"
