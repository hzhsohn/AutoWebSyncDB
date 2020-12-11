<?php
include "module/phpqrcode/qrlib.php"; 
require_once("module/Smarty-3.1.16/libs/Smarty.class.php");
require_once("module/session.m.php");
require_once("module/mysql.m.php");
require_once("_config.php");

error_reporting(E_ALL^E_WARNING);
set_time_limit(0);
ob_clean();



//检查文件夹
function zhPhpMkDir($directoryName)
{
		$directoryName = str_replace("\\","/",$directoryName);
		$dirNames = explode('/', $directoryName);
		$total = count($dirNames) ;
		$temp = '';
		for($i=0; $i<$total; $i++) {
			$temp .= $dirNames[$i].'/';
			if (!is_dir($temp)) {
				$oldmask = umask(0);
				if (!mkdir($temp, 0777)) exit("不能建立目录 $temp"); 
				umask($oldmask);
			}
		}
		return true;
}

//获得所有文件
function  zhShowFileCount($dir) {
    $handle=opendir($dir);   
	$i=0;   
	while($file=readdir($handle)) {   
	  if (($file!=".")and($file!="..")) {   
	  $list[$i]=$file;   
	  $i=$i+1;   
	  }   
  }   
  closedir($handle);  
  return $i;
}

//delete所有文件
function  zhDeleteAllFiles($dir) {
  $handle=opendir($dir);   
  $i=0;   
  while($file=readdir($handle)) {   
  if (($file!=".")and($file!="..")) {   
  $list[$i]=$file;   
  $i=$i+1;   
  }   
  }   
  closedir($handle);  
  if($list)
  {
	foreach($list as $df)
	{
	  $delfn="$dir/$df";
	  unlink($delfn);  
	}
  }
  //重定向浏览器 
  header("Location: ?");
}

//删除文件
$method=@$_REQUEST['method'];
if(0==strcmp($method,'del_all') )
{
	zhDeleteAllFiles('./qr_img/');
}
$data=@$_REQUEST['data'];
/*
    $data 数据
    $filename 保存的图片名称
    $errorCorrectionLevel 错误处理级别
    $matrixPointSize 每个黑点的像素
    $margin 图片外围的白色边框像素
*/
zhPhpMkDir('./qr_img/');
if(strcmp($data,''))
{
    $i=0;
    $ex=str_replace("\r\n",",",$data);
    $ex=explode(',',$ex);
    //var_dump($ex);

    $db=new PzhMySqlDB();		
    $destex=array();
    foreach($ex as $skey)
    {
      $scanKey=trim($skey);
      if($scanKey!='')
      {
          $db->open_mysql(cfg_db_host,cfg_db,cfg_db_username,cfg_db_passwd);
          $db->query("call sp_new_scankey('$scanKey')",0,0);
          if($rs=$db->read())
          {
            if(1==$rs[0])
            {
                //echo $scanKey.' 添加成功';
                $insertOK=1;
            }
            elseif(2==$rs[0])
            {
                //echo $scanKey.' 已经存在';
                $insertOK=0;
            }
           $destex[]=array('key'=>$scanKey,'insert_ok'=>$insertOK);
          }
          $db->close();
      }
    }
}


?>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
</head>
<body>
<table width="80%" border="1" align="center" cellpadding="5" cellspacing="0">
<form action="?" method="post">
  <tr>
    <td>
      <p>请填入内容,内容不允许有逗号,多行即生成多个二维码</p>
      <p>
        <label>
          <textarea name="data" cols="50" rows="10" id="data" style="width:100%"><?=$data?></textarea>
        </label>
      </p>
    </td>
    <td width="25%" align="center"><input type="submit" name="button" id="button" value=" 生成二维码 "  style="width:100px;height:100px"/></td>
    <td width="25%" align="center"><p>
      <input type="button" name="button2" id="button2" value="删除缓存文件"  style="width:100px;height:100px" onClick="location.href='?method=del_all'"/>
    </p>
      <p>当前文件数量:<span id="imgcount"><?=zhShowFileCount('./qr_img/')?></span></p></td>
  </tr>
  </form>
</table>
</p>

<table width="80%" border="1" align="center" cellpadding="5" cellspacing="0">
  <tr>
    <td width="9%">&nbsp;</td>
    <td width="16%"><strong>名称</strong></td>
    <td width="15%"><strong>二维码</strong></td>
    <td width="60%"><strong>备注</strong></td>
  </tr>

<?php
if(!empty($destex)>0)
{
foreach($destex as $dst)
{
  $aa=trim($dst['key']);
  $bb=trim($dst['insert_ok']);
  if($aa!='')
  {
    $filename='./qr_img/'.urlencode($aa).".png";
    $errorCorrectionLevel="L"; 
    $matrixPointSize="5";
    $margin="2";
    QRcode::png($aa, $filename, $errorCorrectionLevel, $matrixPointSize, $margin);
    //echo $aa;
	
?><tr>
    <td align="center"><strong><?=$i+1?></strong></td>
    <td><?=$aa?></td>
    <td><?="<img src=\"$filename\" width=60/>"?></td>
    <td><?=$bb?'插入成功':'记录已存在'?></td>
  </tr>
<?php
    $i++;
    }	
}
}
?>
</table>
<script language="javascript">
var imgCount=parseInt(document.getElementById("imgcount").innerHTML);
imgCount+=<?=$i?>;
document.getElementById("imgcount").innerHTML=imgCount;
</script>
</body>
</html>