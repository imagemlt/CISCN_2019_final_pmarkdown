<!DOCTYPE html>
<html lang="zh-CN">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>MarkDiary</title>
    <script src="https://cdn.jsdelivr.net/npm/jquery@1.12.4/dist/jquery.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@3.3.7/dist/js/bootstrap.min.js"></script>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@3.3.7/dist/css/bootstrap.min.css" rel="stylesheet">
</head>
<body style="background-image: url(img/bg.jpg);background-attachment:fixed">
    <div class="container" style="opacity:0.7;">
        <h1 class="page-header"><strong>MarkDiary</strong></h1>
        <div>
            <?php 
            if(isset($_GET['act'])){
                include($_GET['act'].'.php'); 
            }
            else{
                include('list.php');
            }
            ?>
        </div>
    </div>


  </body>
</html>

