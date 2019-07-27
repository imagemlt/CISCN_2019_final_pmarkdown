--TEST--
Check if pmarkdown is loaded
--SKIPIF--
<?php
if (!extension_loaded('pmarkdown')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "pmarkdown" is available';
?>
--EXPECT--
The extension "pmarkdown" is available
