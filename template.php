<?php
/**
 * @file
 * susy is a tool-kit for Drupal front-end development.
 *
 * DON'T MODIFY susy UNLESS IT'S AN IMPROVMENT - IN THIS CASE, LET US KNOW.
 */

$path_to_susy = drupal_get_path('theme', 'susy');
require_once $path_to_susy . '/includes/preprocess.inc'; // Preprocess hooks
require_once $path_to_susy . '/includes/theme.inc';      // Theme functions
