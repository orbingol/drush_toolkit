<?php

/**
 * @file
 * Utility commands for Drush.
 */

/**
 * Implementation of hook_drush_command().
 */
function orb_drush_command() {

  $items = array();

  $items['orb-load-node'] = array(
    'description' => 'Dumps node array elements to screen',
    'alias' => array('orb-ln'),
    'arguments' => array(
      'nid' => 'Node ID to load',
    ),
  );

  $items['orb-load-node-by-ct'] = array(
    'description' => 'Load nodes by content types and count characters in the body field.',
    'alias' => array('orb-lnct'),
    'arguments' => array(
      'ct' => 'Machine name of the content type to load',
      'lang' => 'Language to be counted. Default: und. Possible values: tr, en, fr',
    ),
  );

  $items['orb-load-user'] = array(
    'description' => 'Dumps user array elements to screen',
    'alias' => array('orb-lu'),
    'arguments' => array(
      'uid' => 'User ID or user name to load',
    ),
  );

  return $items;
}

/**
 * Drush command to get field details of a node entity.
 */
function drush_orb_load_node($nid = NULL) {

  if ($nid == NULL || !is_numeric($nid)) {
    // drush_set_error always returns false
    return drush_set_error('error', t('@nid is not a valid node ID', array('@nid' => $nid)));
  }

  // Load node by node ID value
  $node = node_load($nid);

  if (!empty($node)) {
    // Print the output
    drush_print_r($node);
  }
  else {
    return drush_set_error('error', t('Node @nid is not available', array('@nid' => $nid)));
  }

}

/**
 * Drush command to get character count of the body field for selected content types.
 */
function drush_orb_load_node_by_ct($ct = NULL, $lang = 'und') {

  if ($ct == NULL) {
    // drush_set_error always returns false
    return drush_set_error('error', t('No content type was selected.'));
  }

  // Initialize counters
  $count_published = 0;
  $count_unpublished = 0;
  $length_published = 0;
  $length_published_notags = 0;
  $length_unpublished = 0;
  $length_unpublished_notags = 0;

  // Load all nodes by content type
  $nodes = node_load_multiple(array(), array('type' => $ct));

  // If there are no nodes, put a warning and exit
  if (empty($nodes)) {
    // drush_set_error always returns false
    return drush_set_error('warning', t('No nodes are present in @content_type', array('@content_type' => $ct)));
  }

  // Print some informative messages about language
  drush_print(t('Processing language "@lang"', array('@lang' => $lang)));

  // Loop through all loaded nodes
  foreach ($nodes as $n) {

    // Choose published nodes first
    if ($n->status == 1) {

      // Increase published nodes counter
      $count_published++;
      // Measure the length of body
      $length_published += strlen($n->body[$lang][0]['value']);
      // It is good to measure with no html tags
      $notags_body = strip_tags($n->body[$lang][0]['value']);
      // Measure the length of body with no html tags
      $length_published_notags += strlen($notags_body);

    }
    else { // Process unpublished ones last

      // Increase unpublished nodes counter
      $count_unpublished++;
      // Measure the length of body
      $length_unpublished = $length_unpublished + strlen($n->body[$lang][0]['value']);
      // It is good to measure with no html tags
      $notags_body = strip_tags($n->body[$lang][0]['value']);
      // Measure the length of body with no html tags
      $length_unpublished_notags += strlen($notags_body);

    }
  }

  // Print the findings
  drush_print(t('Total @content_type published: @count', array(
    '@content_type' => $ct,
    '@count' => (int) $count_published
  )));
  drush_print(t('Total @content_type unpublished: @count', array(
    '@content_type' => $ct,
    '@count' => (int) $count_unpublished
  )));
  drush_print(t('Total characters (published with tags): @length', array('@length' => $length_published)));
  drush_print(t('Total characters (published with no tags): @length', array('@length' => $length_published_notags)));
  drush_print(t('Total characters (unpublished with tags): @length', array('@length' => $length_unpublished)));
  drush_print(t('Total characters (unpublished with no tags): @length', array('@length' => $length_unpublished_notags)));

}

/**
 * Drush command to get field details of a user entity.
 */
function drush_orb_load_user($uid = NULL) {

  if ($uid == NULL) {
    // drush_set_error always returns false
    return drush_set_error('error', t('Specify User ID or user name to load'));
  }

  if (is_numeric($uid) && $uid > 0) {
    // Load user by user ID value
    $user = user_load($uid);
  }
  else {
    // Load user by user name
    $user = user_load_by_name($uid);
  }

  if (!empty($user)) {
    // Print the output
    drush_print_r($user);
  }
  else {
    return drush_set_error('error', t('No such user (@uid)', array('@uid' => $uid)));
  }

}