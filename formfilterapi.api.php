<?php

/**
 * @file
 * Hooks provided by the Formfilterapi module.
 */

/**
 * Form to filter data.
 *
 * Via this hook possible to change variables before processing their module.
 * And add your form elements, for filtering.
 *
 * @param string $op
 *   Name of the operation is executed.
 *
 * @param string $session_name
 *   The session name form.
 *
 * @param array $sessvals
 *   Field before filtration.
 */
function hook_formfilterapi($op, $session_name, &$sessvals = array()) {
  $example_session_name = 'test';

  switch ($op) {
    case 'pre_query':
      if ($session_name == $example_session_name) {
        $sessvals['title'] = '%' . $sessvals['title'] . '%';
      }
      break;

    case 'filters':
      if ($session_name == $example_session_name) {
        $form['title'] = array(
          '#type' => 'textfield',
          '#title' => t('Title'),
          '#where' => "n.title LIKE ?",
        );

        return $form;
      }
      break;
  }
}
