<?php
error_reporting(0);
//include("flag.php");
if(isset($_FILES['file'])){
    if($_SERVER['REMOTE_ADDR']!='127.0.0.1'){
    file_put_contents('/tmp/sadfdsf','ip');
        header('Location:index.php?act=upload&mess=ip');
        exit;
    }
    $post_name=pathinfo($_FILES['file']['name'])['filename'].'.md';
    move_uploaded_file($_FILES['file']['tmp_name'],'posts/'.$post_name);
    
    header('Location:index.php?act=upload&mess=success');
}       
?>
        
        
            <form action='upload.php' method="post" enctype="multipart/form-data">
        选择文件:<input type="file" name="file" /><br />
                <input type="submit" value="上传" />
            </form>

<?php if(isset($_GET['mess'])){
    if($_GET['mess']=='success'){
        ?>
            <p style="font-size: 24px;">File Uploaded Success</p>
        <?php

    }
    elseif($_GET['mess']=='ip'){
        ?>
         <p style="font-size: 24px;">Access Forbidden!</p>
            <p style="font-size: 24px;">Only By LoCalH0sT</p>
            
            <?php
    }
}
           

?>

