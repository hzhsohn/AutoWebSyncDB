<?php
$key=$_SERVER['QUERY_STRING'];
$filen="./db.txt";

$fp=fopen($filen,"r");
if($fp)
{
  $json=fread($fp,filesize($filen));
  $json=json_decode($json);
  if($json)
  {
    //var_dump($json);
    $obj=$json->$key;  
    if($obj)
    {
      echo '{"ret":"ok","msg":"",';
      $ipv=$obj->ipv;
      $uptime=$obj->uptime;
      echo "\"key\":\"$key\",\"time\":\"$uptime\",\"ip\":\"$ipv\"";
      echo '}';
    }
    else
    {
      echo '{"ret":"fail","msg":"not found key '.$key.'"}';
    }
  }
  else
  {
    echo '{"ret":"fail","msg":"err db data"}';
  }
}
else
{
  echo '{"ret":"fail","msg":"no db files"}';
}
fclose($fp);


?>