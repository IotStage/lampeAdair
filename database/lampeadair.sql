-- phpMyAdmin SQL Dump
-- version 4.0.10deb1
-- http://www.phpmyadmin.net
--
-- Client: localhost
-- Généré le: Ven 10 Février 2017 à 12:16
-- Version du serveur: 5.5.54-0ubuntu0.14.04.1-log
-- Version de PHP: 5.5.9-1ubuntu4.20

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Base de données: `iotStage`
--

-- --------------------------------------------------------

--
-- Structure de la table `lampeadair`
--

CREATE TABLE IF NOT EXISTS `lampeadair` (
  `id` bigint(50) NOT NULL AUTO_INCREMENT,
  `mesure` double NOT NULL,
  `date_heure` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=3 ;

--
-- Contenu de la table `lampeadair`
--

INSERT INTO `lampeadair` (`id`, `mesure`, `date_heure`) VALUES
(1, 20, '2017-02-09 22:42:30'),
(2, 20, '2017-02-09 23:11:56');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
