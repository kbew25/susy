<?php
/**
 * @file
 * This drush script creates a sub-theme for susy.
 * Usage: drush nst "Theme Name"
 * Started as a patch by ssherriff (http://drupal.org/user/92501).
 * @see http://drupal.org/node/1350038
 *
 */

/**
 * Implementation of hook_drush_command().
 */
function susy_drush_command() {
  $items = array();

  $items['susy-new-subtheme'] = array(
    'description' => 'Set up a susy starter kit.',
    'aliases' => array('nst'),
    'arguments' => array(
      'name' => 'A name for your theme.',
      'machine_name' => '(optional)[a-z, 0-9] A machine-readable name for your theme.',
    ),
    'examples' => array(
      'example[1]' => 'drush nst "My theme name"',
    ),
  );

  $items['susy-compile'] = array(
    'description' => 'Compile a theme.',
    'aliases' => array('sw'),
    'arguments' => array(
      'name' => 'Your theme machine-name.',
    ),
    'examples' => array(
      'example[1]' => 'drush sw my_theme_name',
    ),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function susy_drush_help($section) {
  switch ($section) {
    case 'drush:susy-new-subtheme':
      return dt("Set up a susy starter kit.");
    case 'drush:susy-compile':
      return dt("Compile a theme.");
  }
}


/**
 * Command: Compile a theme.
 */
function drush_susy_compile($name = NULL) {
  if (empty($name)) {
    drush_print(dt("\nWe can't compile a theme with no name. \nPlease provide a theme name.\nEx. drush sw [name]\n"));
    return;
  }

  // Do we have a config.rb file?
  if (!($path = drush_core_find_project_path($name)) || !file_exists($path . '/config.rb')) {
    drush_print(dt("\nCould not compile !name !path\n",
      array(
        '!name' => $name,
        '!path' => $path ? ' (' . $path . ')' : '',
      )
    ));
    return -1;
  }

  return (int) drush_shell_exec("compass compile %s", $path);
}


/**
 * Command: Set up a susy starter kit.
 */
function drush_susy_new_subtheme($name = NULL, $machine_name = NULL) {
  if (empty($name)) {
    drush_print(dt("\nWe can't create a subtheme with no name. \nPlease provide a name for the subtheme.\nEx. drush nst [name]\n"));
    return;
  }

  $machine_name = !empty($machine_name) ? preg_replace('/[^a-z0-9_]+/', '', strtolower($machine_name)) : preg_replace('/[^a-z0-9_]+/', '', strtolower($name));

  if (drupal_get_path('theme', 'susy') == '') {
    drush_print(dt("\nWe could not find the path to susy. \nPlease clear your cache and try again.\n"));
    return;
  }

  // Find Drupal install and susy paths
  $root_path = drush_locate_root();
  $susy_relative = drupal_get_path('theme', 'susy');

  // Construct relative path for new subtheme config
  $_susy_relative = explode('/', $susy_relative);
  array_pop($_susy_relative);
  $themes_relative = implode('/', $_susy_relative);
  $subtheme_relative = $themes_relative . '/' . $machine_name;

  // Construct absolute paths for copying
  $susy_path = $root_path  . '/' . $susy_relative;
  $subtheme_path = $root_path . '/' . $subtheme_relative;

  // Make a fresh copy of the original starter kit.
  $s = drush_copy_dir("$susy_path/SUBTHEME/", $subtheme_path);
  if (empty($s)) {
    return;
  }

  // Rename the info file and fill in the theme name.
  drush_op('rename', "$subtheme_path/SUBTHEME.info", "$subtheme_path/$machine_name.info");
  drush_op('susy_file_str_replace', "$subtheme_path/$machine_name.info", 'SUBTHEME', "$machine_name");

  // Notify user of the newly created theme.
  drush_print(dt("\n!name sub-theme was created in !path. \n",
    array(
      '!name' => $name,
      '!path' => $subtheme_path,
    )
  ));

  drush_invoke_process("@self", "pm-enable", array($machine_name));

  if (drush_confirm(dt("\nWould you like to set !name as your default theme?", array('!name' => $name)))) {
    drush_op('variable_set', 'theme_default', $machine_name);
    drush_print(dt("!name was set as your default theme. \n", array('!name' => $name)));
  }

}


/**
 * Internal helper: Replace strings in a file.
 */
function susy_file_str_replace($file_path, $find, $replace) {
  $file_contents = file_get_contents($file_path);
  $file_contents = str_replace($find, $replace, $file_contents);
  file_put_contents($file_path, $file_contents);
}
