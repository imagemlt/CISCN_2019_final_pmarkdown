<ul class="list-group">
               
            
            <?php
            foreach(scandir('/var/www/html/posts') as $file){
                if(substr($file,-3)=='.md'){
                    ?>
                        <li class="list-group-item">
                        <span class="badge">新</span>
                        <a href="/index.php?act=post&md=<?php echo $file; ?>"><?php echo $file;?></a>
                        </li>
                    <?php
                }
            }
            ?>

            <li class="list-group-item">
            <span class="badge">上传</span>
            <a href="/index.php?act=upload">上传日志</a>
            </li>
            </ul>
