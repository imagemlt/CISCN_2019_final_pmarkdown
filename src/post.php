<div style='background-color:rgba(255,255,255,1);border-radius:5px'>
<?php
if(isset($_GET['md'])){
	if(substr($_GET['md'],-3)!='.md')exit;
	pmark_include('posts/'.$_GET['md']);
?>
<textarea style='visibility:hidden'>
 <?php	include("posts/$_GET[md]");?>
</textarea>
<?php

}
?>
</div>
