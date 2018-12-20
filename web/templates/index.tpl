{config_load file="a.conf"}
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>{#title#}</title>
<style type="text/css">
<!--

#mid #left {
	width: 22%;
	margin-right: auto;
	border-right-style: solid;
	border-right-width: 2px;
	border-right-color: #999;
	font-family: Tahoma, Geneva, sans-serif;
	font-size: 12px;
}

#mid #right {
	width: 79%;
	margin-left: auto;
	height: 380px;
}

#mid #nav2 {
	width: 76%;
}

#mid #nav2 .subtype {
	background-color: #DDD;
	width: 32%;
	font-family: Tahoma, Geneva, sans-serif;
	font-size: 14px;
	font-weight: normal;
	color: #363;
	padding-bottom: 5px;
}

#mid #nav2 .subtype li{
	padding-bottom: 2px;
	border-bottom-width: 1px;
	border-bottom-style: dashed;
	border-bottom-color: #666;
	margin-bottom: 8px;
	padding-top: 3px;
}

#mid nav2 ol {
	color: #111;
	font-weight: normal;
	font-family: Tahoma, Geneva, sans-serif;
}

.fltrt { /* 此类可用来使页面中的元素向右浮动。浮动元素必须位于页面上要与之相邻的元素之前。 */
	float: right;
	margin-left: 8px;
}
.fltlft { /* 此类可用来使页面上的元素向左浮动 */
	float: left;
	margin-right: 8px;
}

.fr { /* 此类可用来使页面中的元素向右浮动。浮动元素必须位于页面上要与之相邻的元素之前。 */
	float: right;
	width:19%;
	text-align: right;
}
.fl { /* 此类可用来使页面上的元素向左浮动 */
	float: left;
	width:80%;
}

.clearfloat { /* 此类应当放在 div 或 break 元素上，而且该元素应当是完全包含浮动的容器关闭之前的最后一个元素 */
	clear:both;
    height:0;
    font-size: 1px;
    line-height: 0px;
}

#footer {
	width: 100%; /* 自动边距（与宽度一起）会将页面居中 */
	margin-top: 28px;
	margin-right: auto;
	margin-bottom: 28px;
	margin-left: auto;
	border-top-width: 2px;
	border-top-style: solid;
	border-top-color: #999;
	font-family: Tahoma, Geneva, sans-serif;
	font-size: 12px;
	color: #666;
	height: 50px;
	padding-top: 10px;
	text-align: center;
}

#mid a:link {
 color: #000000;
 TEXT-DECORATION: none
}
#mid a:visited {
 COLOR: #000000;
 TEXT-DECORATION: none
}
#mid a:hover {
 COLOR: #ff7f24;
 text-decoration: underline;
}
#mid a:active {
 COLOR: #ff7f24;  
 text-decoration: underline;
}

#mid #left ul li {
	padding-bottom: 6px;
}
#msg #chat ul {
	margin: 0px;
	padding: 0px;
	list-style-type: none;
}

#msg {
	width: 960px; /* 自动边距（与宽度一起）会将页面居中 */
	margin-top: 0px;
	margin-right: auto;
	margin-left: auto;
	border-top-width: 2px;
	border-top-style: solid;
	border-top-color: #690;
	font-family: Tahoma, Geneva, sans-serif;
	font-size: 12px;
}

-->
</style>
</head>

<body bgcolor="#F8F8F8">
{include file="pub_top.tpl"} 
<div id="mid">
  <div id="left" class="fltlft">
  {include file="pub_left_nav.tpl"}
  </div>
  <div id="nav2" class="fltrt"></div>
  <br class="clearfloat" />
</div>
<div id="footer">{#copyright#}</div>
</body>
</html>