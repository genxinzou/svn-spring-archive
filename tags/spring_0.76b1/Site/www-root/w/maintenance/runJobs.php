<?php

$optionsWithArgs = array( 'maxjobs' );
$wgUseNormalUser = true;
require_once( 'commandLine.inc' );
require_once( "$IP/includes/JobQueue.php" );
require_once( "$IP/includes/FakeTitle.php" );

if ( isset( $options['maxjobs'] ) ) {
	$maxJobs = $options['maxjobs'];
} else {
	$maxJobs = 10000;
}

$wgTitle = Title::newFromText( 'RunJobs.php' );

$dbw = wfGetDB( DB_MASTER );
$n = 0;
while ( $dbw->selectField( 'job', 'count(*)', '', 'runJobs.php' ) ) {
	$offset=0;
	while ( false != ($job = Job::pop($offset)) ) {
		wfWaitForSlaves( 5 );
		print $job->id . "  " . $job->toString() . "\n";
		$offset=$job->id;
		if ( !$job->run() ) {
			print "Error: {$job->error}\n";
		}
		if ( $maxJobs && ++$n > $maxJobs ) {
			break 2;
		}
	}
}
?>
