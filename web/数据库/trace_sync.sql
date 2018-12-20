/*
Navicat MySQL Data Transfer

Source Server         : 景容
Source Server Version : 50637
Source Host           : 119.23.36.154:3306
Source Database       : trace_sync

Target Server Type    : MYSQL
Target Server Version : 50637
File Encoding         : 65001

Date: 2018-12-20 17:57:21
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
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of tb_append
-- ----------------------------
INSERT INTO `tb_append` VALUES ('16', 'a003', '7e9f8a119eece537f07fe4bdf009685f', '{\"operater\":\"11\",\"time\":\"2018-12-20\",\"batch\":\"\",\"place\":\"\"}', '223.74.168.50', '2018-12-20 17:54:12');
INSERT INTO `tb_append` VALUES ('17', 'a003', '16f2e537ca5b844ae57c1a0fc8a0cdf1', '{\"operater\":\"12\",\"time\":\"2018-12-20\",\"batch\":\"\",\"place\":\"\"}', '223.74.168.50', '2018-12-20 17:54:31');

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
INSERT INTO `tb_board` VALUES ('a001', '{\"operater\":\"\",\"time\":\"2018-12-20\",\"batch\":\"\",\"place\":\"\"}', '223.74.168.50', '2018-12-20 17:23:48', '2018-12-20 17:44:21');
INSERT INTO `tb_board` VALUES ('a002', 'okok111k', '123', '2018-12-20 17:23:48', '2018-12-20 17:24:33');
INSERT INTO `tb_board` VALUES ('a003', '{\"operater\":\"\",\"time\":\"2018-12-20\",\"batch\":\"\",\"place\":\"\"}', '223.74.168.50', '2018-12-20 17:23:48', '2018-12-20 17:45:37');
INSERT INTO `tb_board` VALUES ('a004', 'okok111k', '123', '2018-12-20 17:24:07', '2018-12-20 17:24:17');

-- ----------------------------
-- Procedure structure for `sp_add_data`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_add_data`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_add_data`(in update1_or_append2 tinyint,in _ip varchar(46),in _scankey varchar(46),in _json varchar(2048) CHARACTER SET utf8)
    COMMENT '1=添加成功\r\n2=无效scankey\r\n3=追加内容重复'
begin

declare result_ tinyint;

if exists(select * from tb_board where scan_key=_scankey) then
	if 1=update1_or_append2 then
		update tb_board set json=_json,uptime=now(),ip=_ip where scan_key=_scankey ;
		set result_=1;
	elseif 2=update1_or_append2 then
		if not exists(select * from tb_append where json_md5=md5(_json)) then
					insert into tb_append(ip,scan_key,json_md5,json,uptime) 
								    values(_ip , _scankey ,md5(_json), _json,now());
					set result_=1;
		ELSE					
					set result_=3;
		end if;
	end if;
else
	set result_=2;
end if;

select result_;

end
;;
DELIMITER ;
