<?php

function getIP() /*获取客户端IP*/ 
{ 
if (@$_SERVER["HTTP_X_FORWARDED_FOR"]) 
$ip = $_SERVER["HTTP_X_FORWARDED_FOR"]; 
else if (@$_SERVER["HTTP_CLIENT_IP"]) 
$ip = $_SERVER["HTTP_CLIENT_IP"]; 
else if (@$_SERVER["REMOTE_ADDR"]) 
$ip = $_SERVER["REMOTE_ADDR"]; 
else if (@getenv("HTTP_X_FORWARDED_FOR")) 
$ip = getenv("HTTP_X_FORWARDED_FOR"); 
else if (@getenv("HTTP_CLIENT_IP")) 
$ip = getenv("HTTP_CLIENT_IP"); 
else if (@getenv("REMOTE_ADDR")) 
$ip = getenv("REMOTE_ADDR"); 
else 
$ip = "Unknown"; 
return $ip; 
} 

//---------------------------------------------------
echo '<?xml version="1.0" encoding="utf-8"?>';
$title=$_SERVER['QUERY_STRING'];
if(0==strcmp($title,''))
{
	echo '{"ret": "ok","act":"create file"}';
	exit;
}
$ipv=getIP();

//-------------------------
ob_clean();
date_default_timezone_set('PRC'); //获取系统时间
define("_now",date("Y-m-d H:i:s",time()));
//------------------------
//重新写入到文件
$filename="./db.txt";
$fp=@fopen($filename,"r");
if($fp)
{
	$json = fread($fp, filesize($filename));
}
@fclose($fp);
//var_dump($json);

$isUpdate=0;
$fp=fopen($filename,"w");
$json=json_decode($json, true);
if($json["$title"]!="")
{
	$isUpdate=1;
}

//var_dump($json);
$json["$title"]=array('ipv'=>$ipv,'uptime'=>_now);
$strJ=json_encode($json);
//echo $strJ;
fprintf($fp,"%s",$strJ);
fclose($fp);
if($isUpdate)
{
  //不存在于数据库的key
  //echo '{"ret": "invalid","act":"update"}';
	echo '{"ret": "ok","act":"update"}';
}
else
{
	echo '{"ret": "ok","act":"insert"}';
}
?>