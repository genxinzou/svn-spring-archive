<?php
	# Mantis - a php based bugtracking system
	# Copyright (C) 2000 - 2002  Kenzaburo Ito - kenito@300baud.org
	# Copyright (C) 2002 - 2005  Mantis Team   - mantisbt-dev@lists.sourceforge.net
	# This program is distributed under the terms and conditions of the GPL
	# See the README and LICENSE files for details

	# --------------------------------------------------------
	# $Id: csv_export.php,v 1.25 2005/06/08 22:09:13 vboctor Exp $
	# --------------------------------------------------------
?>
<?php
	require_once( 'core.php' );

	$t_core_path = config_get( 'core_path' );

	require_once( $t_core_path . 'filter_api.php' );
	require_once( $t_core_path . 'csv_api.php' );
	require_once( $t_core_path . 'columns_api.php' );
?>
<?php auth_ensure_user_authenticated() ?>
<?php
	helper_begin_long_process();

	$t_page_number = 1;
	$t_per_page = -1;
	$t_bug_count = null;
	$t_page_count = null;

	$t_nl = csv_get_newline();
 	$t_sep = csv_get_separator();

	# Get bug rows according to the current filter
	$t_rows = filter_get_bug_rows( $t_page_number, $t_per_page, $t_page_count, $t_bug_count );
	if ( $t_rows === false ) {
		print_header_redirect( 'view_all_set.php?type=0' );
	}

	$t_filename = csv_get_default_filename();

	# Send headers to browser to activate mime loading

	# Make sure that IE can download the attachments under https.
	header( 'Pragma: public' );

	header( 'Content-Type: text/plain; name=' . $t_filename );
	header( 'Content-Transfer-Encoding: BASE64;' );

	# Added Quotes (") around file name.
	header( 'Content-Disposition: attachment; filename="' . $t_filename .'"');

	# Get columns to be exported
	$t_columns = csv_get_columns();

	# export the titles
	$t_first_column = true;
	ob_start();
	$t_titles = array();
	foreach ( $t_columns as $t_column ) {
		if ( !$t_first_column ) {
			echo $t_sep;
		} else {
			$t_first_column = false;
		}

		if ( strpos( $t_column, 'custom_' ) === 0 ) {
			$t_column_title_function = 'print_column_title';
			helper_call_custom_function( $t_column_title_function, array( $t_column, COLUMNS_TARGET_CSV_PAGE ) );
		} else {
			$t_function = 'print_column_title_' . $t_column;
			$t_function( '', 'ASC', COLUMNS_TARGET_CSV_PAGE );
		}
	}

	echo $t_nl;

	$t_header = ob_get_clean();

	# Fixed for a problem in Excel where it prompts error message "SYLK: File Format Is Not Valid"
	# See Microsoft Knowledge Base Article - 323626
	# http://support.microsoft.com/default.aspx?scid=kb;en-us;323626&Product=xlw
	$t_first_three_chars = substr( $t_header, 0, 3 );
	if ( strcmp( $t_first_three_chars, 'ID' . $t_sep ) == 0 ) {
		$t_header = str_replace( 'ID' . $t_sep, 'Id' . $t_sep, $t_header );
	}
	# end of fix

	echo $t_header;

	# export the rows
	foreach ( $t_rows as $t_row ) {
		$t_first_column = true;

		foreach ( $t_columns as $t_column ) {
			if ( !$t_first_column ) {
				echo $t_sep;
			} else {
				$t_first_column = false;
			}

			if ( strpos( $t_column, 'custom_' ) === 0 ) {
				ob_start();
				$t_column_value_function = 'print_column_value';
				helper_call_custom_function( $t_column_value_function, array( $t_column, $t_row, COLUMNS_TARGET_CSV_PAGE ) );
				$t_value = ob_get_clean();

				if ( strstr( $t_value, $t_sep ) !== false ) {
					$t_value = '"' . $t_value . '"';
				}

				echo $t_value;
			} else {
				$t_function = 'csv_format_' . $t_column;
				echo $t_function( $t_row[ $t_column ] );
			}
		}

		echo $t_nl;
	}
?>
