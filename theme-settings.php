<?php

/**
 * @file
 * Theme settings for the susy
 */
function susy_form_system_theme_settings_alter(&$form, &$form_state) {

  global $theme_key;

  /**
   * HTML5 IE support
   */
  $form['susy_settings']['susy_html5'] = array(
    '#type' => 'fieldset',
    '#title' => t('HTML5 IE support'),
  );

  $form['susy_settings']['susy_html5']['susy_force_ie'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force latest IE rendering engine (even in intranet) & Chrome Frame'),
    '#default_value' => theme_get_setting('susy_force_ie'),
  );

  $form['susy_settings']['susy_html5']['susy_html5shiv'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable HTML5 elements in IE'),
    '#description' => t('Makes IE understand HTML5 elements via <a href="!shivlink">HTML5 shiv</a>. disable if you use a different method.', array('!shivlink' => 'http://code.google.com/p/html5shiv/')),
    '#default_value' => theme_get_setting('susy_html5shiv'),
  );

  $form['susy_settings']['susy_html5']['susy_ie_comments'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add IE specific classes'),
    '#description' => t('This will add conditional classes to the html tag for IE specific styling. see this <a href="!post">post</a> for more info.', array('!post' => 'http://paulirish.com/2008/conditional-stylesheets-vs-css-hacks-answer-neither/')),
    '#default_value' => theme_get_setting('susy_ie_comments'),
  );
  
  $form['susy_settings']['susy_html5']['susy_prompt_cf'] = array(
    '#type' => 'select',
    '#title' => t('Prompt old IE users to update'),
    '#default_value' => theme_get_setting('susy_prompt_cf'),
    '#options' => drupal_map_assoc(array(
       'Disabled',
       'IE 6',
       'IE 7',
       'IE 8',
       'IE 9',
    )),
      '#description' => t('Set the latest IE version you would like the prompt box to show on or disable if you want to support old IEs.'),
  );
}
