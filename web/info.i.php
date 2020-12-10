<?php
/**************************************

Author:Han.zhihong
remark:
数据显示页

***************************************/

require_once("module/mysql.m.php");
require_once("_config.php");

$scan_key=$_SERVER['QUERY_STRING'];
if(0==strcmp($scan_key,''))
{
  echo 'no parameter';
	exit;
}

$db=new PzhMySqlDB();		
$db->open_mysql(cfg_db_host,cfg_db,cfg_db_username,cfg_db_passwd);
$db->query("select*from tb_board where scan_key='$scan_key'",0,0);
if($rs=$db->read())
{
	//var_dump($rs);
 	$j=json_decode($rs['json'],true);
}
if(null==$j)
{
	echo 'not found scan key';
	exit;
}
//var_dump($j);

$db->query("select*from tb_append where scan_key='$scan_key'",0,0);
$l=array();
while($rs=$db->read())
{
 	$l[]=json_decode($rs['json'],true);
}
$db->close();
//var_dump($l);


$j['list']=$l;
echo json_encode($j);;

?>