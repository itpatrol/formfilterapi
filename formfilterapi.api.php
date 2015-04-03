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
 * @param string $operation
 *   Name of the operation is executed.
 * @param string $session_name
 *   The unique session name.
 *
 * @return array
 *   Form with filter elements.
 */
function hook_formfilterapi($operation, $session_name) {
  switch ($operation) {
    case 'filters':
      if ($session_name == 'test') {
        $form['title'] = array(
          '#type' => 'textfield',
          '#title' => t('Title'),
          '#where' => array('like' => 'n.title'),
        );

        return $form;
      }
      break;
  }
}

/**
 * Alter filters for hook_formfilterapi().
 *
 * @param array $filters
 *  Filters from hook_formfilterapi().
 * @param string $session_name
 *   The unique session name.
 * @param string $operation
 *   Name of the operation is executed.
 */
function hook_formfilterapi_alter(&$filters, $session_name, $operation) {
  switch ($operation) {
    case 'filters':
      if ($session_name == 'test') {
        $filters['sticky'] = array(
          '#type' => 'select',
          '#title' => t('Sticky at top of lists'),
          '#options' => array(
            '1' => t('Yes'),
            '0' => t('No'),
          ),
          '#where' => 'n.sticky',
        );
      }
      break;
  }
}
