-- phpMyAdmin SQL Dump
-- version 4.2.11
-- http://www.phpmyadmin.net
--
-- Хост: 127.0.0.1
-- Время создания: Фев 16 2016 г., 21:48
-- Версия сервера: 5.6.21
-- Версия PHP: 5.6.3

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- База данных: `smarthouse`
--
CREATE DATABASE IF NOT EXISTS `smarthouse` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
USE `smarthouse`;

-- --------------------------------------------------------

--
-- Структура таблицы `action`
--

DROP TABLE IF EXISTS `action`;
CREATE TABLE IF NOT EXISTS `action` (
`idaction` int(11) NOT NULL,
  `description` varchar(80) DEFAULT NULL,
  `action` varchar(45) DEFAULT NULL,
  `param` varchar(200) DEFAULT NULL,
  `param1` varchar(80) DEFAULT NULL,
  `param2` varchar(80) DEFAULT NULL,
  `type` varchar(45) NOT NULL,
  `equipment_idequipment` int(11) NOT NULL
) ENGINE=InnoDB AUTO_INCREMENT=31 DEFAULT CHARSET=utf8;

--
-- Дамп данных таблицы `action`
--

INSERT INTO `action` (`idaction`, `description`, `action`, `param`, `param1`, `param2`, `type`, `equipment_idequipment`) VALUES
(1, 'Сказать время', NULL, NULL, NULL, NULL, 'saytime', 2),
(2, 'Мелодия будильника', NULL, 'bud.wav', NULL, NULL, 'play', 2),
(3, 'Включить правый свет в большой комнате', NULL, '01set1222', NULL, NULL, 'send', 3),
(4, 'Сказать температуру', NULL, NULL, NULL, NULL, 'saytemp', 2),
(5, 'Взвести таймер закрытия замка', '=', '6', NULL, NULL, 'var', 5),
(6, 'Отсчет таймера закрытия замка', '-', '1', NULL, NULL, 'var', 5),
(7, 'Выключение таймера закрытия замка', '=', '-1', NULL, NULL, 'var', 5),
(8, 'Включить дергалку замка', NULL, '03set222222222221', NULL, NULL, 'send', 3),
(9, 'Выключить дергалку замка', NULL, '03set222222222220', NULL, NULL, 'send', 3),
(10, 'Включить ДД в прихожей', NULL, '03set2222222221', NULL, NULL, 'send', 3),
(11, 'Выключить ДД в прихожей', NULL, '03set2222222220', NULL, NULL, 'send', 3),
(14, 'Выключить дергалку слива в туалете', NULL, '06set2202', NULL, NULL, 'send', 3),
(15, 'тестовое действие', NULL, 'INSERT INTO `smarthouse`.`events` (`idevents`, `time`, `description`, `link_idlink`) VALUES (NULL, ''2015-10-21 00:00:00'', ''gasdfasdfasdf'', ''5'')', NULL, NULL, 'sql', 2),
(16, 'Отсчет таймера света в туалете', '-', '1', NULL, NULL, 'var', 14),
(17, 'Выключить свет, сенсор и вентиляцию в туалете', NULL, '06set222022222200', NULL, NULL, 'send', 3),
(18, 'Выключение таймера света в туалете', '=', '-1', NULL, NULL, 'var', 14),
(19, 'Взведение таймера света в туалете', '=', '240', NULL, NULL, 'var', 14),
(20, 'Включить свет, сенсор и вентиляцию в туалете', NULL, '06set222122222211', NULL, NULL, 'send', 3),
(21, 'Включить свет в прихожей', NULL, '03set2221', NULL, NULL, 'send', 3),
(22, 'Выключить свет в прихожей', NULL, '03set2220', NULL, NULL, 'send', 3),
(23, 'Выключить ручное управление жалюзями', NULL, '02set2222222222220', NULL, NULL, 'send', 3),
(24, 'Прекратить открытие жалюзей', NULL, '02set2212', NULL, NULL, 'send', 3),
(25, 'Прекратить закрытие жалюзей', NULL, '02set2221', NULL, NULL, 'send', 3),
(26, 'Закрыть жалюзи', NULL, '02set2220', NULL, NULL, 'send', 3),
(27, 'Открыть жалюзи', NULL, '02set2202', NULL, NULL, 'send', 3),
(28, 'Включить дергалку слива в туалете', NULL, '06set2212', NULL, NULL, 'send', 3),
(29, 'Взвести флаг сенсора в туалете', '=', '1', NULL, NULL, 'var', 19),
(30, 'Выключение флага сенсора в туалете', '=', '-1', NULL, NULL, 'var', 19);

-- --------------------------------------------------------

--
-- Структура таблицы `condition`
--

DROP TABLE IF EXISTS `condition`;
CREATE TABLE IF NOT EXISTS `condition` (
`idcondition` int(11) NOT NULL,
  `description` varchar(80) DEFAULT NULL,
  `condition` varchar(45) DEFAULT NULL,
  `param` varchar(200) DEFAULT NULL,
  `param1` varchar(80) DEFAULT NULL,
  `param2` varchar(80) DEFAULT NULL,
  `type` varchar(45) NOT NULL,
  `equipment_idequipment` int(11) NOT NULL
) ENGINE=InnoDB AUTO_INCREMENT=40 DEFAULT CHARSET=utf8;

--
-- Дамп данных таблицы `condition`
--

INSERT INTO `condition` (`idcondition`, `description`, `condition`, `param`, `param1`, `param2`, `type`, `equipment_idequipment`) VALUES
(1, 'Если время больше 12.30', '>', '12.30', NULL, NULL, 'time', 1),
(2, 'Если не суббота', '!', '6', NULL, NULL, 'day', 1),
(3, 'Если не воскресенье', '!', '7', NULL, NULL, 'day', 1),
(4, 'Если время первого будильника', '=', '6.30', NULL, NULL, 'time', 1),
(5, 'Если время второго будильника', '=', '6.40', NULL, NULL, 'time', 1),
(6, 'Если время третьего будильника', '=', '6.50', NULL, NULL, 'time', 1),
(7, 'Открытие входной двери', '>1', NULL, NULL, NULL, 'pin', 4),
(8, 'Закрытие входной двери', '>0', NULL, NULL, NULL, 'pin', 4),
(9, 'Пока работает таймер закрытия замка', '>', '-1', NULL, NULL, 'var', 5),
(10, 'Таймаут таймера закрытия замка', '=', '0', NULL, NULL, 'var', 5),
(11, 'Если входная дверь закрыта', '=0', NULL, NULL, NULL, 'pin', 4),
(12, 'Замок входной двери открыт', '=1', NULL, NULL, NULL, 'pin', 6),
(13, 'Если дергалка включена', '=1', NULL, NULL, NULL, 'pin', 8),
(14, 'Если время больше 6.00', '>', '6.00', NULL, NULL, 'time', 1),
(15, 'Если время меньше 6.00', '<', '6.00', NULL, NULL, 'time', 1),
(16, 'Если светло', '<', '200', NULL, NULL, 'analog', 7),
(17, 'Если темно', '>', '270', NULL, NULL, 'analog', 7),
(18, 'Если ДД в прихожей выключен', '=0', NULL, NULL, NULL, 'pin', 9),
(19, 'Если ДД в прихожей включен', '=1', NULL, NULL, NULL, 'pin', 9),
(20, 'Если сработал датчик протечки в туалете', '>0', NULL, NULL, NULL, 'pin', 10),
(21, 'Если дергалка в туалете включена', '=1', NULL, NULL, NULL, 'pin', 11),
(22, 'Пока работает таймер света в туалете', '>', '0', NULL, NULL, 'var', 14),
(23, 'Таймаут таймера света в туалете', '=', '0', NULL, NULL, 'var', 14),
(24, 'Сработка ДД в туалете', '>1', NULL, NULL, NULL, 'pin', 12),
(25, 'Открылась дверь в туалет', '>1', NULL, NULL, NULL, 'pin', 13),
(26, 'Если входная дверь открыта', '=1', NULL, NULL, NULL, 'pin', 4),
(27, 'Если включилось ручное управление жалюзями', '>1', NULL, NULL, NULL, 'pin', 15),
(28, 'Если жалюзи открываются', '=0', NULL, NULL, NULL, 'pin', 17),
(29, 'Если вЫключено ручное управление жалюзями', '=0', NULL, NULL, NULL, 'pin', 15),
(30, 'Если жалюзи закрываются', '=0', NULL, NULL, NULL, 'pin', 16),
(31, 'Если нажата кнопка слива в туалете', '>0', NULL, NULL, NULL, 'pin', 18),
(32, 'Если ЗАЖАТА кнопка слива в туалете', '=0', NULL, NULL, NULL, 'pin', 18),
(33, 'Если дергалка в туалете выключилась', '>0', NULL, NULL, NULL, 'pin', 11),
(34, 'Если отпущена кнопка слива в туалете', '>1', NULL, NULL, NULL, 'pin', 18),
(35, 'Если дергалка в туалете включилась', '>1', NULL, NULL, NULL, 'pin', 11),
(36, 'Если свет в туалете включился', '>1', NULL, NULL, NULL, 'pin', 20),
(37, 'Если взведен флаг сенсора в туалете', '=', '1', NULL, NULL, 'var', 19),
(38, 'Если время больше 23.00', '>', '23.00', NULL, NULL, 'time', 1),
(39, 'Если время меньше 23.00', '<', '23.00', NULL, NULL, 'time', 1);

-- --------------------------------------------------------

--
-- Структура таблицы `equipment`
--

DROP TABLE IF EXISTS `equipment`;
CREATE TABLE IF NOT EXISTS `equipment` (
`idequipment` int(11) NOT NULL,
  `description` varchar(80) DEFAULT NULL,
  `type` varchar(45) NOT NULL,
  `controller` int(11) DEFAULT NULL,
  `pintype` int(11) DEFAULT NULL,
  `pinnumber` int(11) DEFAULT NULL,
  `addr` varchar(45) DEFAULT NULL,
  `object` varchar(280) DEFAULT NULL
) ENGINE=InnoDB AUTO_INCREMENT=22 DEFAULT CHARSET=utf8;

--
-- Дамп данных таблицы `equipment`
--

INSERT INTO `equipment` (`idequipment`, `description`, `type`, `controller`, `pintype`, `pinnumber`, `addr`, `object`) VALUES
(1, 'Часы реального времени', 'time', NULL, NULL, NULL, NULL, NULL),
(2, 'Аудиоплеер', 'player', NULL, NULL, NULL, NULL, NULL),
(3, 'Отправка команд в шину', 'pincommander', NULL, NULL, NULL, NULL, NULL),
(4, 'Геркон входной двери', 'pin', 3, 3, 0, NULL, NULL),
(5, 'переменная таймера закрытия замка', 'var', NULL, NULL, NULL, '10', '-1'),
(6, 'Геркон замка входной двери', 'pin', 3, 3, 3, NULL, NULL),
(7, 'Датчик освещенности на балконе', 'analog', 5, NULL, NULL, NULL, NULL),
(8, 'Дергалка замка', 'pin', 3, 1, 11, NULL, NULL),
(9, 'Управление ДД в прихожей', 'pin', 3, 1, 9, NULL, NULL),
(10, 'Датчик протечки в туалете', 'pin', 6, 3, 2, NULL, NULL),
(11, 'Дергалка слива в туалете', 'pin', 6, 1, 2, NULL, NULL),
(12, 'ДД в туалете', 'pin', 6, 2, 3, NULL, NULL),
(13, 'Геркон в туалете', 'pin', 6, 3, 1, NULL, NULL),
(14, 'Таймер света в туалете', 'var', NULL, NULL, NULL, '3', '-1'),
(15, 'Ручное управление жалюзями', 'pin', 2, 1, 12, NULL, NULL),
(16, 'Открытие жалюзей', 'pin', 2, 1, 3, NULL, NULL),
(17, 'Закрытие жалюзей', 'pin', 2, 1, 2, NULL, NULL),
(18, 'Кнопка слива в туалете', 'pin', 6, 2, 2, NULL, NULL),
(19, 'Флаг активации сенсора в туалете', 'var', NULL, NULL, NULL, '4', NULL),
(20, 'Свет в туалете', 'pin', 6, 1, 3, NULL, NULL),
(21, 'Тестовая переменная', 'var', NULL, NULL, NULL, '6', 'abs(analog(5))');

-- --------------------------------------------------------

--
-- Структура таблицы `events`
--

DROP TABLE IF EXISTS `events`;
CREATE TABLE IF NOT EXISTS `events` (
`idevents` int(11) NOT NULL,
  `time` datetime DEFAULT NULL,
  `description` varchar(205) DEFAULT NULL,
  `link_idlink` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Структура таблицы `link`
--

DROP TABLE IF EXISTS `link`;
CREATE TABLE IF NOT EXISTS `link` (
`idlink` int(11) NOT NULL,
  `description` varchar(205) DEFAULT NULL,
  `option` varchar(45) DEFAULT NULL,
  `param` int(11) DEFAULT NULL,
  `event_gen_onstart` tinyint(1) DEFAULT NULL,
  `event_gen_onend` tinyint(1) DEFAULT NULL,
  `enabled` tinyint(1) NOT NULL
) ENGINE=InnoDB AUTO_INCREMENT=34 DEFAULT CHARSET=utf8;

--
-- Дамп данных таблицы `link`
--

INSERT INTO `link` (`idlink`, `description`, `option`, `param`, `event_gen_onstart`, `event_gen_onend`, `enabled`) VALUES
(1, 'Тестовая линка', 'timeout', 10000, 0, 0, 0),
(2, 'Будильник 1', 'timeout', 31000, 0, 1, 1),
(3, 'Будильник 2', 'timeout', 31000, 0, 1, 1),
(4, 'Будильник 3', 'timeout', 31000, 0, 1, 1),
(5, 'Говорить время и температуру', 'timeout', 1800000, 0, 0, 1),
(6, 'Таймер закрытия замка', 'timeout', 100, 0, 0, 1),
(7, 'Отсчет таймера закрытия замка', 'oncecheck', 1000, 0, 0, 1),
(8, 'Действие таймера закрытия замка', 'timeout', 5000, 0, 1, 1),
(9, 'Включилась дергалка замка, выключаю', 'oncecheck', 300, 0, 1, 1),
(10, 'Стало темно, включение ДД в прихожей, закрываю жалюзи', 'timeout', 60000, 0, 1, 1),
(11, 'Стало светло, выключение ДД в прихожей, открываю жалюзи', 'timeout', 60000, 0, 1, 1),
(12, 'Выключение ДД в прихожей на ночь', 'timeout', 5000, 0, 1, 1),
(13, 'Закрытие входной двери', 'timeout', 100, 0, 1, 1),
(14, 'Открытие входной двери', 'timeout', 100, 0, 1, 1),
(15, 'СРАБОТАЛ ДАТЧИК ПРОТЕЧКИ В ТУАЛЕТЕ!', 'timeout', 1000, 0, 1, 1),
(16, 'Выключение слива в туалете', 'doafter', 1500, 0, 0, 1),
(17, 'Отсчет таймера света в туалете', 'timeout', 1000, 0, 0, 1),
(18, 'Действие таймера света в туалете', 'timeout', 5000, 0, 0, 1),
(19, 'Включение света в туалете по ДД', 'timeout', 100, 0, 0, 1),
(20, 'Включение света в туалете по открытию двери', 'timeout', 100, 0, 0, 1),
(21, 'Включение света при открытии входной двери', 'timeout', 100, 0, 0, 1),
(22, 'Выключение света после закрытия входной двери', 'doafter', 45000, 0, 0, 1),
(23, 'Автовыключение ручного управления жалюзями', 'doafter', 10000, 0, 1, 1),
(24, 'Автовыключение открытия жалюзей', 'oncecheck', 150, 0, 0, 1),
(25, 'Автовыключение закрытия жалюзей', 'oncecheck', 150, 0, 0, 1),
(26, 'Дергание дергалки слива по сенсору', 'timeout', 1000, 0, 1, 1),
(27, 'Аварийное выключение слива в туалете', 'oncecheck', 5000, 0, 0, 1),
(28, 'Дергалка слива активна', 'timeout', 10, 0, 1, 0),
(29, 'Дергалка слива пассивна', 'timeout', 10, 0, 1, 0),
(30, 'Нажатие кнопки слива', 'timeout', 10, 0, 1, 0),
(31, 'Отпущена кнопка слива', 'timeout', 10, 0, 1, 0),
(32, 'Взвести флаг сенсора в туалете', 'oncecheck', 2000, 0, 0, 1),
(33, 'Выключение ДД в прихожей на ночь', 'timeout', 5000, 0, 1, 1);

-- --------------------------------------------------------

--
-- Структура таблицы `link_has_action`
--

DROP TABLE IF EXISTS `link_has_action`;
CREATE TABLE IF NOT EXISTS `link_has_action` (
  `link_idlink` int(11) NOT NULL,
  `action_idaction` int(11) NOT NULL,
  `sort_index` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Дамп данных таблицы `link_has_action`
--

INSERT INTO `link_has_action` (`link_idlink`, `action_idaction`, `sort_index`) VALUES
(1, 1, 0),
(1, 15, 1),
(2, 1, 2),
(2, 2, 1),
(2, 3, 3),
(3, 1, 2),
(3, 2, 1),
(3, 3, 3),
(4, 1, 2),
(4, 2, 1),
(4, 3, 3),
(5, 1, 1),
(5, 4, 2),
(6, 5, 1),
(7, 6, 1),
(8, 7, 1),
(8, 8, 2),
(9, 9, 0),
(10, 10, 1),
(10, 26, 2),
(11, 11, 1),
(11, 27, 2),
(12, 11, 1),
(12, 26, 2),
(16, 14, 1),
(17, 16, 1),
(18, 17, 3),
(18, 18, 2),
(18, 30, 1),
(19, 19, 1),
(19, 20, 2),
(20, 19, 1),
(20, 20, 2),
(21, 21, 1),
(22, 22, 1),
(23, 23, 1),
(24, 24, 1),
(25, 25, 1),
(26, 28, 1),
(27, 14, 1),
(32, 29, 1),
(33, 11, 1),
(33, 26, 2);

-- --------------------------------------------------------

--
-- Структура таблицы `link_has_condition`
--

DROP TABLE IF EXISTS `link_has_condition`;
CREATE TABLE IF NOT EXISTS `link_has_condition` (
  `link_idlink` int(11) NOT NULL,
  `condition_idcondition` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Дамп данных таблицы `link_has_condition`
--

INSERT INTO `link_has_condition` (`link_idlink`, `condition_idcondition`) VALUES
(1, 1),
(2, 2),
(3, 2),
(4, 2),
(2, 3),
(3, 3),
(4, 3),
(2, 4),
(3, 5),
(4, 6),
(5, 7),
(14, 7),
(21, 7),
(6, 8),
(13, 8),
(7, 9),
(8, 10),
(8, 11),
(8, 12),
(9, 13),
(10, 14),
(11, 14),
(12, 15),
(11, 16),
(10, 17),
(10, 18),
(11, 19),
(12, 19),
(33, 19),
(15, 20),
(27, 21),
(17, 22),
(18, 23),
(19, 24),
(20, 25),
(22, 26),
(23, 27),
(24, 28),
(24, 29),
(25, 29),
(25, 30),
(26, 31),
(30, 31),
(16, 32),
(29, 33),
(31, 34),
(28, 35),
(32, 36),
(26, 37),
(33, 38),
(10, 39),
(11, 39);

--
-- Индексы сохранённых таблиц
--

--
-- Индексы таблицы `action`
--
ALTER TABLE `action`
 ADD PRIMARY KEY (`idaction`), ADD KEY `fk_action_equipment1_idx` (`equipment_idequipment`);

--
-- Индексы таблицы `condition`
--
ALTER TABLE `condition`
 ADD PRIMARY KEY (`idcondition`), ADD KEY `fk_condition_equipment1_idx` (`equipment_idequipment`);

--
-- Индексы таблицы `equipment`
--
ALTER TABLE `equipment`
 ADD PRIMARY KEY (`idequipment`);

--
-- Индексы таблицы `events`
--
ALTER TABLE `events`
 ADD PRIMARY KEY (`idevents`), ADD KEY `fk_events_link1_idx` (`link_idlink`);

--
-- Индексы таблицы `link`
--
ALTER TABLE `link`
 ADD PRIMARY KEY (`idlink`);

--
-- Индексы таблицы `link_has_action`
--
ALTER TABLE `link_has_action`
 ADD PRIMARY KEY (`link_idlink`,`action_idaction`), ADD KEY `fk_link_has_action_action1_idx` (`action_idaction`), ADD KEY `fk_link_has_action_link1_idx` (`link_idlink`);

--
-- Индексы таблицы `link_has_condition`
--
ALTER TABLE `link_has_condition`
 ADD PRIMARY KEY (`link_idlink`,`condition_idcondition`), ADD KEY `fk_link_has_condition_condition1_idx` (`condition_idcondition`), ADD KEY `fk_link_has_condition_link1_idx` (`link_idlink`);

--
-- AUTO_INCREMENT для сохранённых таблиц
--

--
-- AUTO_INCREMENT для таблицы `action`
--
ALTER TABLE `action`
MODIFY `idaction` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=31;
--
-- AUTO_INCREMENT для таблицы `condition`
--
ALTER TABLE `condition`
MODIFY `idcondition` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=40;
--
-- AUTO_INCREMENT для таблицы `equipment`
--
ALTER TABLE `equipment`
MODIFY `idequipment` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=22;
--
-- AUTO_INCREMENT для таблицы `events`
--
ALTER TABLE `events`
MODIFY `idevents` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT для таблицы `link`
--
ALTER TABLE `link`
MODIFY `idlink` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=34;
--
-- Ограничения внешнего ключа сохраненных таблиц
--

--
-- Ограничения внешнего ключа таблицы `action`
--
ALTER TABLE `action`
ADD CONSTRAINT `fk_action_equipment1` FOREIGN KEY (`equipment_idequipment`) REFERENCES `equipment` (`idequipment`) ON DELETE NO ACTION ON UPDATE NO ACTION;

--
-- Ограничения внешнего ключа таблицы `condition`
--
ALTER TABLE `condition`
ADD CONSTRAINT `fk_condition_equipment1` FOREIGN KEY (`equipment_idequipment`) REFERENCES `equipment` (`idequipment`) ON DELETE NO ACTION ON UPDATE NO ACTION;

--
-- Ограничения внешнего ключа таблицы `events`
--
ALTER TABLE `events`
ADD CONSTRAINT `fk_events_link1` FOREIGN KEY (`link_idlink`) REFERENCES `link` (`idlink`) ON DELETE NO ACTION ON UPDATE NO ACTION;

--
-- Ограничения внешнего ключа таблицы `link_has_action`
--
ALTER TABLE `link_has_action`
ADD CONSTRAINT `fk_link_has_action_action1` FOREIGN KEY (`action_idaction`) REFERENCES `action` (`idaction`) ON DELETE NO ACTION ON UPDATE NO ACTION,
ADD CONSTRAINT `fk_link_has_action_link1` FOREIGN KEY (`link_idlink`) REFERENCES `link` (`idlink`) ON DELETE NO ACTION ON UPDATE NO ACTION;

--
-- Ограничения внешнего ключа таблицы `link_has_condition`
--
ALTER TABLE `link_has_condition`
ADD CONSTRAINT `fk_link_has_condition_condition1` FOREIGN KEY (`condition_idcondition`) REFERENCES `condition` (`idcondition`) ON DELETE NO ACTION ON UPDATE NO ACTION,
ADD CONSTRAINT `fk_link_has_condition_link1` FOREIGN KEY (`link_idlink`) REFERENCES `link` (`idlink`) ON DELETE NO ACTION ON UPDATE NO ACTION;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;