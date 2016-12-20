<?php

/**
 * @file
 * Prepare variables for template files.
 */

/**
 * Implements template_preprocess_html().
 */
function susy_preprocess_html(&$vars) {

  $vars['doctype'] = _susy_doctype();
  $vars['rdf'] = _susy_rdf($vars);
  $vars['html_attributes'] = 'lang="' . $vars['language']->language . '" dir="' . $vars['language']->dir . '" ' . $vars['rdf']->version . $vars['rdf']->namespaces;

  // IE coditional comments on the <html> tag
  // http://paulirish.com/2008/conditional-stylesheets-vs-css-hacks-answer-neither/
  if (theme_get_setting('susy_ie_comments')) {
    $vars['html'] = '<!--[if lte IE 7 ]><html ' . $vars['html_attributes'] . ' class="no-js ie7 lte-ie9 lte-ie8"><![endif]-->
                     <!--[if IE 8 ]><html ' . $vars['html_attributes'] . ' class="no-js ie8 lte-ie9 lte-ie8"><![endif]-->
                     <!--[if IE 9 ]><html ' . $vars['html_attributes'] . ' class="no-js ie9 lte-ie9"><![endif]-->
                     <!--[if gt IE 9]><!--> <html ' . $vars['html_attributes'] . ' class="no-js"> <!--<![endif]-->';
  } else {
    $vars['html'] = '<html ' . $vars['html_attributes'] . ' class="no-js">';
  }

  // Enable HTML5 elements in IE
  $vars['html5shiv'] = theme_get_setting('susy_html5shiv') ? '<!--[if lt IE 9]><script src="'. base_path() . drupal_get_path('theme', 'susy') .'/scripts/html5shiv.js"></script><![endif]-->' : '';

  // Force latest IE rendering engine
  if (theme_get_setting('susy_force_ie')) {
    $meta_force_ie = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'http-equiv' => 'X-UA-Compatible',
        'content' =>  'IE=edge,chrome=1',
      )
    );
    drupal_add_html_head($meta_force_ie, 'meta_force_ie');
  }

  // Change the default meta content-type tag to the shorter HTML5 version
  $charset = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'charset' => 'utf-8',
    )
  );
  drupal_add_html_head($charset, 'system_meta_content_type');

  // Prompt IE users to install Chrome Frame
  if (theme_get_setting('susy_prompt_cf') != 'Disabled') {
    $vars['prompt_cf'] = "<!--[if lte " . theme_get_setting('susy_prompt_cf') . " ]>
      <div style='background: #eee; text-align: center; padding: 3px;'>" . t("Your browser is <b>out of date</b> and may <b>not display all features</b> of this and other websites. !url", array('!url' => l(t('Learn how to update your browser'), "http://outdatedbrowser.com/", array('attributes' => array('target'=>'_blank'))))) . "</div><![endif]-->";
  } else {
    $vars['prompt_cf'] = '';
  }

  // Since menu is rendered in preprocess_page we need to detect it here to add body classes
  $has_main_menu = theme_get_setting('toggle_main_menu');
  $has_secondary_menu = theme_get_setting('toggle_secondary_menu');

  /* Add extra classes to body for more flexible theming */

  if ($has_main_menu or $has_secondary_menu) {
    $vars['classes_array'][] = 'with-navigation';
  }

  if ($has_secondary_menu) {
    $vars['classes_array'][] = 'with-subnav';
  }

  if (!empty($vars['page']['featured'])) {
    $vars['classes_array'][] = 'featured';
  }

  if ($vars['is_admin']) {
    $vars['classes_array'][] = 'admin';
  }

  $vars['classes_array'][] = 'dir-' . $vars['language']->dir;

  if (!$vars['is_front']) {
    // Add unique classes for each page and website section
    $path = drupal_get_path_alias($_GET['q']);
    $temp = explode('/', $path, 2);
    $section = array_shift($temp);
    $page_name = array_shift($temp);

    if (isset($page_name)) {
      $vars['classes_array'][] = drupal_html_id('page-' . $page_name);
    }

    $vars['classes_array'][] = drupal_html_id('section-' . $section);

    if (arg(0) == 'node') {
      if (arg(1) == 'add') {
        if ($section == 'node') {
          array_pop($vars['classes_array']); // Remove 'section-node'
        }
        $vars['classes_array'][] = 'section-node-add'; // Add 'section-node-add'
      } elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
        if ($section == 'node') {
          array_pop($vars['classes_array']); // Remove 'section-node'
        }
        $vars['classes_array'][] = 'section-node-' . arg(2); // Add 'section-node-edit' or 'section-node-delete'
      }
    }
  }

  susy_css_include();
}


/**
 * Implements template_preprocess_page().
 */
function susy_preprocess_page(&$vars) {

  if (isset($vars['node_title'])) {
    $vars['title'] = $vars['node_title'];
  }

  // Site navigation links.
  $vars['main_menu_links'] = '';
  if (!empty($vars['main_menu'])) {
    $main_menu_name = variable_get('menu_main_links_source', 'main-menu');
    $menu_tree = menu_build_tree($main_menu_name);
    $menu_tree_output = menu_tree_output($menu_tree);
    $menu_rendered = drupal_render($menu_tree_output);
    $vars['main_menu_links'] = $menu_rendered;
  }
  $vars['secondary_menu_links'] = '';
  if (!empty($vars['secondary_menu'])) {
    $secondary_menu_name = variable_get('menu_secondary_links_source', 'user-menu');
    $menu_tree = menu_build_tree($secondary_menu_name);
    $menu_tree_output = menu_tree_output($menu_tree);
    $menu_tree_output['#theme_wrappers'][] = 'menu_tree__secondary_menu';
    $menu_rendered = drupal_render($menu_tree_output);
    $vars['secondary_menu_links'] = $menu_rendered;
  }

  // Since the title and the shortcut link are both block level elements,
  // positioning them next to each other is much simpler with a wrapper div.
  if (!empty($vars['title_suffix']['add_or_remove_shortcut']) && $vars['title']) {
    // Add a wrapper div using the title_prefix and title_suffix render elements.
    $vars['title_prefix']['shortcut_wrapper'] = array(
      '#markup' => '<div class="shortcut-wrapper clearfix">',
      '#weight' => 100,
    );
    $vars['title_suffix']['shortcut_wrapper'] = array(
      '#markup' => '</div>',
      '#weight' => -99,
    );
    // Make sure the shortcut link is the first item in title_suffix.
    $vars['title_suffix']['add_or_remove_shortcut']['#weight'] = -100;
  }

  if(!theme_get_setting('susy_feed_icons')) {
    $vars['feed_icons'] = '';
  }
}


/**
 * Implements template_preprocess_node().
 *
 * Adds extra classes to node container for advanced theming
 */
function susy_preprocess_node(&$vars) {
  // Striping class
  $vars['classes_array'][] = 'node-' . $vars['zebra'];

  // Node is published
  $vars['classes_array'][] = ($vars['status']) ? 'published' : 'unpublished';

  // Node has comments?
  $vars['classes_array'][] = ($vars['comment']) ? 'with-comments' : 'no-comments';

  if ($vars['sticky']) {
    $vars['classes_array'][] = 'sticky'; // Node is sticky
  }

  if ($vars['promote']) {
    $vars['classes_array'][] = 'promote'; // Node is promoted to front page
  }

  if ($vars['teaser']) {
    $vars['classes_array'][] = 'node-teaser'; // Node is displayed as teaser.
  }

  if ($vars['uid'] && $vars['uid'] === $GLOBALS['user']->uid) {
    $classes[] = 'node-mine'; // Node is authored by current user.
  }

  $vars['submitted'] = t('Submitted by !username on ', array('!username' => $vars['name']));
  $vars['submitted_date'] = t('!datetime', array('!datetime' => $vars['date']));
  $vars['submitted_pubdate'] = format_date($vars['created'], 'custom', 'Y-m-d\TH:i:s');

  if ($vars['view_mode'] == 'full' && node_is_page($vars['node'])) {
    $vars['classes_array'][] = 'node-full';
  }
}


/**
 * Implements template_preprocess_block().
 */
function susy_preprocess_block(&$vars, $hook) {
  // Add a striping class.
  $vars['classes_array'][] = 'block-' . $vars['zebra'];

  $vars['title_attributes_array']['class'][] = 'block-title';

  // Use nav element for menu blocks and provide a suggestion for all of them
  $nav_blocks = array('navigation', 'main-menu', 'management', 'user-menu');
  $nav_modules = array('superfish', 'nice_menus', 'menu_block');
  if (in_array($vars['block']->delta, $nav_blocks) || in_array($vars['block']->module, $nav_modules)) {
    $vars['tag'] = 'nav';
    array_unshift($vars['theme_hook_suggestions'], 'block__menu');
  }
  elseif (!empty($vars['block']->subject)) {
    $vars['tag'] = 'section';
  }
  else {
    $vars['tag'] = 'div';
  }

  // In the header region visually hide block titles.
  if ($vars['block']->region == 'header') {
    $vars['title_attributes_array']['class'][] = 'element-invisible';
  }
}


/**
 * Implements template_proprocess_search_block_form().
 *
 * Changes the search form to use the HTML5 "search" input attribute
 */
function susy_preprocess_search_block_form(&$vars) {
  $vars['search_form'] = str_replace('type="text"', 'type="search"', $vars['search_form']);
}


/**
 * Implements template_proprocess_preprocess_menu_local_task().
 *
 * Override or insert variables into theme_menu_local_task().
 */
function susy_preprocess_menu_local_task(&$vars) {
  $link = & $vars['element']['#link'];

  // If the link does not contain HTML already, check_plain() it now.
  // After we set 'html'=TRUE the link will not be sanitized by l().
  if (empty($link['localized_options']['html'])) {
    $link['title'] = check_plain($link['title']);
  }

  $link['localized_options']['html'] = TRUE;
  $link['title'] = '<span class="tab">' . $link['title'] . '</span>';
}


/**
 * Generate doctype for templates
 */
function _susy_doctype() {
  return (module_exists('rdf')) ? '<!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN"' . "\n" . '"http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">' : '<!DOCTYPE html>' . "\n";
}


/**
 * Generate RDF object for templates
 *
 * Uses RDFa attributes if the RDF module is enabled
 * Lifted from Adaptivetheme for D7, full credit to Jeff Burnz
 * ref: http://drupal.org/node/887600
 *
 * @param array $vars
 */
function _susy_rdf($vars) {
  $rdf = new stdClass();

  if (module_exists('rdf')) {
    $rdf->version = 'version="HTML+RDFa 1.1"';
    $rdf->namespaces = $vars['rdf_namespaces'];
    $rdf->profile = ' profile="' . $vars['grddl_profile'] . '"';
  } else {
    $rdf->version = '';
    $rdf->namespaces = '';
    $rdf->profile = '';
  }

  return $rdf;
}

/**
 * Includes all custom style sheets for the current theme.
 */
function susy_css_include() {

  $dynasty = susy_theme_dynasty();

  foreach ($dynasty as $theme) {
    $info = drupal_parse_info_file($theme->filename);

    if (isset($info['styles']) && !empty($info['styles'])) {
      foreach ($info['styles'] as $file => $style) {
        $file = drupal_get_path('theme', $theme->name) . "/{$file}";
        $style['options']['theme'] = $theme->name;
        drupal_add_css($file, $style['options']);
      }
    }
  }
}


/**
 * Build the theme tree from base theme to active theme.
 */
function susy_theme_dynasty() {
  global $theme_key;
  $themes = list_themes();
  $dynasty = array();
  $dynasty[] = $obj = $themes[$theme_key];

  while (isset($obj->base_theme) && isset($themes[$obj->base_theme]) && !empty($themes[$obj->base_theme])) {
    $dynasty[] = $obj = $themes[$obj->base_theme];
  }

  return $dynasty;
}
