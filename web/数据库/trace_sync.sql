/*
Navicat MySQL Data Transfer

Source Server         : 景容
Source Server Version : 50637
Source Host           : 119.23.36.154:3306
Source Database       : trace_sync

Target Server Type    : MYSQL
Target Server Version : 50637
File Encoding         : 65001

Date: 2020-12-10 18:05:17
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
) ENGINE=InnoDB AUTO_INCREMENT=23 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of tb_append
-- ----------------------------
INSERT INTO `tb_append` VALUES ('18', 'a001', '22da80db229c5b1c84d7e39f7d7774af', '{\"operater\":\"123\",\"time\":\"2020-12-07\",\"batch\":\"321\",\"place\":\"123\",\"type\":\"123\"}', '101.247.241.96', '2020-12-07 23:21:53');
INSERT INTO `tb_append` VALUES ('19', 'a001', 'dbd4d37da0f895b24cb9bc4c582cf8a5', '{\"operater\":\"sadfasdf\",\"time\":\"2020-12-08\",\"mark\":\"ssdfadf\",\"place\":\"sdfsaf\",\"type\":\"asdfasdf\"}', '101.247.246.68', '2020-12-08 00:45:41');
INSERT INTO `tb_append` VALUES ('20', 'a001', 'b4bdaebcefecd7140747e9808ee742ee', '{\"operater\":\"小龙666\",\"time\":\"2020-12-08\",\"mark\":\"NB6666\",\"place\":\"\",\"type\":\"\"}', '223.74.122.39', '2020-12-08 15:42:03');
INSERT INTO `tb_append` VALUES ('21', 'a001', '5af6a2f8b1e065a58ecd9139bcadce53', '{\"operater\":\"\",\"time\":\"2020-12-08\",\"mark\":\"\",\"place\":\"\",\"type\":\"\"}', '223.74.122.39', '2020-12-08 15:51:20');
INSERT INTO `tb_append` VALUES ('22', 'a001', '5b0a915c21bebd4ded64f414bb300028', '{\"operater\":\"中合合\",\"time\":\"2020-12-08\",\"mark\":\"在枯霜要要\"}', '223.74.122.39', '2020-12-08 18:11:47');

-- ----------------------------
-- Table structure for `tb_board`
-- ----------------------------
DROP TABLE IF EXISTS `tb_board`;
CREATE TABLE `tb_board` (
  `autoid` bigint(64) unsigned NOT NULL AUTO_INCREMENT,
  `scan_key` varchar(46) NOT NULL,
  `json` varchar(2048) DEFAULT NULL,
  `ip` varchar(46) DEFAULT NULL,
  `create_key_date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `uptime` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`autoid`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of tb_board
-- ----------------------------
INSERT INTO `tb_board` VALUES ('5', 'a001', '{\"operater\":\"哈哈666\",\"time\":\"2020-12-08\",\"batch\":\"最牛逼的批次\",\"place\":\"大中国\",\"type\":\"888\"}', '223.74.122.39', '2020-12-07 23:03:24', '2020-12-08 16:01:48');
INSERT INTO `tb_board` VALUES ('9', '123', null, null, '2020-12-10 18:13:31', null);
INSERT INTO `tb_board` VALUES ('10', '456', null, null, '2020-12-10 18:13:31', null);
INSERT INTO `tb_board` VALUES ('11', '789', null, null, '2020-12-10 18:13:31', null);

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
