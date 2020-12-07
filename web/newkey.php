<?php

require_once("module/Smarty-3.1.16/libs/Smarty.class.php");
require_once("module/session.m.php");
require_once("module/mysql.m.php");
require_once("_config.php");

ob_clean();

if('newkey'==$exc)
{
    $db=new PzhMySqlDB();		
    $db->open_mysql(cfg_db_host,cfg_db,cfg_db_username,cfg_db_passwd);
    $db->query("call sp_new_scankey('$scanKey')",0,0);
    if($rs=$db->read())
    {
      if(1==$rs[0])
      {
          echo $scanKey.' 添加成功';
      }
      elseif(2==$rs[0])
      {
        echo $scanKey.' 已经存在';
      }
    }
    $db->close();
}


?>
