<?php
/**************************************

Author:Han.zhihong
remark:
数据显示页

***************************************/

require_once("module/Smarty-3.1.16/libs/Smarty.class.php");
require_once("module/verifycode.m.php");
require_once("module/session.m.php");
require_once("module/mysql.m.php");
require_once("_config.php");


$db=new PzhMySqlDB();		
$db->open_mysql(cfg_db_host,cfg_db,cfg_db_username,cfg_db_passwd);
$db->query("select*from d_tb_dpid where accountdb_userlist_userid=$userid",0,0);
while($rs=$db->read())
{
 
}
$db->close();

//显示页面
$smarty = new Smarty;
$smarty->force_compile = smarty_force_compile;
$smarty->debugging = smarty_debugging;
$smarty->caching = smarty_caching;
$smarty->cache_lifetime = smarty_cache_lifetime;

$smarty->assign("key",$key);
$smarty->display('index.tpl');

?>