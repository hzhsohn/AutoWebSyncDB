/*
Navicat MySQL Data Transfer

Source Server         : hanzhihong.cn
Source Server Version : 50637
Source Host           : hanzhihong.cn:3306
Source Database       : trace_sync

Target Server Type    : MYSQL
Target Server Version : 50637
File Encoding         : 65001

Date: 2020-12-08 00:47:50
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `tb_append`
-- ----------------------------
DROP TABLE IF EXISTS `tb_append`;
CREATE TABLE `tb_append` (
  `autoid` bigint(64) unsigned NOT NULL AUTO_INCREMENT,
  `scan_key` varchar(46) NOT NULL,
  `json_md5` varchar(36) NOT NULL,
  `json` varchar(2048) NOT NULL,
  `ip` varchar(46) NOT NULL,
  `uptime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`autoid`)
) ENGINE=InnoDB AUTO_INCREMENT=20 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of tb_append
-- ----------------------------
INSERT INTO `tb_append` VALUES ('18', 'a001', '22da80db229c5b1c84d7e39f7d7774af', '{\"operater\":\"123\",\"time\":\"2020-12-07\",\"batch\":\"321\",\"place\":\"123\",\"type\":\"123\"}', '101.247.241.96', '2020-12-07 23:21:53');
INSERT INTO `tb_append` VALUES ('19', 'a001', 'dbd4d37da0f895b24cb9bc4c582cf8a5', '{\"operater\":\"sadfasdf\",\"time\":\"2020-12-08\",\"mark\":\"ssdfadf\",\"place\":\"sdfsaf\",\"type\":\"asdfasdf\"}', '101.247.246.68', '2020-12-08 00:45:41');

-- ----------------------------
-- Table structure for `tb_board`
-- ----------------------------
DROP TABLE IF EXISTS `tb_board`;
CREATE TABLE `tb_board` (
  `scan_key` varchar(46) NOT NULL,
  `json` varchar(2048) DEFAULT NULL,
  `ip` varchar(46) DEFAULT NULL,
  `create_key_date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `uptime` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`scan_key`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of tb_board
-- ----------------------------
INSERT INTO `tb_board` VALUES ('a001', '{\"operater\":\"阿斯蒂芬\",\"time\":\"2020-12-08\",\"batch\":\"asdfasdf\",\"place\":\"123123\",\"type\":\"asdfasdf\"}', '101.247.246.68', '2020-12-07 23:03:24', '2020-12-08 00:30:40');
INSERT INTO `tb_board` VALUES ('n888', null, null, '2020-12-07 23:20:05', null);

-- ----------------------------
-- Procedure structure for `sp_apped_data`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_apped_data`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_apped_data`(in _ip varchar(46),in _scankey varchar(46),in _json varchar(2048) CHARACTER SET utf8)
    COMMENT '1=添加成功\r\n2=无效scankey\r\n3=追加内容重复'
begin

declare result_ tinyint;

if exists(select * from tb_board where scan_key=_scankey) then

		if not exists(select * from tb_append where json_md5=md5(_json)) then
					insert into tb_append(ip,scan_key,json_md5,json,uptime) 
								    values(_ip , _scankey ,md5(_json), _json,now());
					set result_=1;
		ELSE					
					set result_=3;
		end if;

else
	set result_=2;
end if;

select result_;

end
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_new_scankey`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_new_scankey`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_new_scankey`(in _scankey varchar(46))
    COMMENT '1=添加成功\r\n2=scankey已经存在'
begin

declare result_ tinyint;

if not exists(select * from tb_board where scan_key=_scankey) then
		insert into tb_board (scan_key) values(_scankey);
		set result_=1;
else
	set result_=2;
end if;

select result_;

end
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_update_data`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_update_data`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_update_data`(in _ip varchar(46),in _scankey varchar(46),in _json varchar(2048) CHARACTER SET utf8)
    COMMENT '1=添加成功\r\n2=无效scankey\r\n3=追加内容重复'
begin

declare result_ tinyint;

if exists(select * from tb_board where scan_key=_scankey) then

		update tb_board set json=_json,uptime=now(),ip=_ip where scan_key=_scankey ;
		set result_=1;

else
	set result_=2;
end if;

select result_;

end
;;
DELIMITER ;
