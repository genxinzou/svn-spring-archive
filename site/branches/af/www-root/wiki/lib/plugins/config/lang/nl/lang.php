<?php
/**
 * dutch language file
 *
 * @license    GPL 2 (http://www.gnu.org/licenses/gpl.html)
 * @author Pieter van der Meulen <pieter@vdmeulen.net>
 * @author Wouter Schoot <wouter@schoot.org>
 * @author John de Graaff <john@de-graaff.net>
 */
$lang['menu']                  = 'Configuratie instellingen';
$lang['error']                 = 'De instellingen zijn niet aangebracht wegens een niet correcte waarde, kijk svp je wijzigingen na en sla dan opnieuw op.<br />Je kunt de incorrecte waarde herkennen aan de rode rand.';
$lang['updated']               = 'Instellingen met succes opgeslagen.';
$lang['nochoice']              = '(geen andere keuzemogelijkheden)';
$lang['locked']                = 'Het bestand met instellinegn kan niet worden gewijzigd. Als dit niet de bedeoeling is, <br />zorg dan dat naam en permissies voor het lokale installingen bestand kloppen.';
$lang['_configuration_manager'] = 'Configuratiie manager';
$lang['_header_dokuwiki']      = 'DokuWiki instellingen';
$lang['_header_plugin']        = 'Plugin instellingen';
$lang['_header_template']      = 'Sjabloon instellingen';
$lang['_header_undefined']     = 'Ongedefinierde instellingen';
$lang['_basic']                = 'Basis instellingen';
$lang['_display']              = 'Beeld instellingen';
$lang['_authentication']       = 'Toegangsverificatie instellingen';
$lang['_anti_spam']            = 'Anti-Spam instellingen';
$lang['_editing']              = 'Pagina-wijzigings instellingen';
$lang['_links']                = 'Link instellingen';
$lang['_media']                = 'Media instellingen';
$lang['_advanced']             = 'Geavanceerde instellingen';
$lang['_network']              = 'Netwerk instellingen';
$lang['_plugin_sufix']         = 'Plugin instellingen';
$lang['_template_sufix']       = 'Sjabloon instellingen';
$lang['_msg_setting_undefined'] = 'Geen metedata voor deze instelling.';
$lang['_msg_setting_no_class'] = 'Geen class voor deze instelling.';
$lang['_msg_setting_no_default'] = 'Geen standaard waarde.';
$lang['fmode']                 = 'Bestand aanmaak modus (file creation mode)';
$lang['dmode']                 = 'Directory aanmaak modus (directory creation mode)';
$lang['lang']                  = 'Taal';
$lang['basedir']               = 'Basis directory';
$lang['baseurl']               = 'Basis URL';
$lang['savedir']               = 'Directory om data op te slaan';
$lang['start']                 = 'Start pagina naam';
$lang['title']                 = 'Wiki titel';
$lang['template']              = 'Sjabloon';
$lang['fullpath']              = 'Volledig pad van pagina\'s in de footer weergeven';
$lang['recent']                = 'Recente wijzigingen';
$lang['breadcrumbs']           = 'Aantal broodkruimels';
$lang['youarehere']            = 'Hierarchische broodkruimels';
$lang['typography']            = 'Breng typografische wijzigingen aan';
$lang['htmlok']                = 'Embedded HTML toestaan';
$lang['phpok']                 = 'Embedded PHP toestaan';
$lang['dformat']               = 'Datum formaat (zie de PHP <a href="http://www.php.net/strftime">strftime</a> functie)';
$lang['signature']             = 'Ondertekening';
$lang['toptoclevel']           = 'Bovenste niveau voor inhoudsopgave';
$lang['maxtoclevel']           = 'Laagste niveau voor inhoudsopgave';
$lang['maxseclevel']           = 'Laagste sectiewijzigingsniveau';
$lang['camelcase']             = 'CamelCase gebruiken voor links';
$lang['deaccent']              = 'Paginanamen ontdoen van niet-standaard tekens';
$lang['useheading']            = 'Eerste kopje voor paginanaam gebruiken';
$lang['refcheck']              = 'Controleer verwijzingen naar media';
$lang['refshow']               = 'Aantal te tonen media verwijzigen';
$lang['allowdebug']            = 'Debug toestaan <b>uitzetten indien niet noodzakelijk!</b>';
$lang['usewordblock']          = 'Blokkeer spam op basis van woordenlijst';
$lang['indexdelay']            = 'Uitstel alvorens te indexeren (sec)';
$lang['relnofollow']           = 'Gebruik rel="nofollow" voor externe links';
$lang['mailguard']             = 'Eemail adressen onherkenbaar maken';
$lang['iexssprotect']          = 'Controleer geuploade bestanden voor mogelijk schadelijke JavaScript of HTML code';
$lang['useacl']                = 'Gebruik access control lists';
$lang['autopasswd']            = 'Zelf wachtwoorden genereren';
$lang['authtype']              = 'Authenticatie mechanisme';
$lang['passcrypt']             = 'Wachtwoord encryptie methode';
$lang['defaultgroup']          = 'Standaard groep';
$lang['superuser']             = 'Superuser - een groep of gebruiker met volledige toegang tot alle pagina\'s en functies ongeacht de ACL instellingen';
$lang['manager']               = 'Beheerder - een groep of gebruiker met toegang tot bepaalde beheersfunctionaliteit';
$lang['profileconfirm']        = 'Bevestig profielwijzigingen met wachtwoord';
$lang['disableactions']        = 'Aangevinkte Dokuwiki akties uitschakelen';
$lang['disableactions_check']  = 'Controleer';
$lang['disableactions_subscription'] = 'Inschrijven/opzeggen';
$lang['disableactions_nssubscription'] = 'Namespace inschrijven/uitschrijven';
$lang['disableactions_wikicode'] = 'Bron bekijken/exporteer rauw';
$lang['disableactions_other']  = 'Andere akties (gescheiden door komma)';
$lang['sneaky_index']          = 'Met de standaard instellingen zal DokuWiki alle namespaces laten zien in het index overzicht. Het inschakelen van deze optie zorgt ervoor dat de namespaces waar de gebruiker geen lees toegang tot heeft, verborgen worden. Dit kan resulteren in het verbergen of ontoegankelijk maken van bepaalde subnamespaces. Dit kan de index onbruikbaar maken met bepaalde ACL instellingen.';
$lang['auth_security_timeout'] = 'Authenticatie Beveigings Timeout (seconden)';
$lang['updatecheck']           = 'Controleer op nieuwe versies en beveiligingswaarschuwingen? DokuWiki moet hiervoor contact opnemen met splitbrain.org.';
$lang['userewrite']            = 'Gebruik nette URL\'s';
$lang['useslash']              = 'Gebruik slash (/) als scheifing tussen namepaces in URL\'s';
$lang['usedraft']              = 'Sla automatisch een concept op tijdens het wijzigen';
$lang['sepchar']               = 'Pagina naam woordscheiding';
$lang['canonical']             = 'Herleid URL\'s tot hun basisvorm';
$lang['autoplural']            = 'Controleer op meervoudsvormen in links';
$lang['compression']           = 'Compressie methode voor attic bestanden';
$lang['cachetime']             = 'Maximum leeftijd voor cache (sec)';
$lang['locktime']              = 'Maximum leeftijd voor lock bestanden (sec)';
$lang['fetchsize']             = 'Maximum grootte (bytes) die fetch.php mag downloaden van buiten';
$lang['notify']                = 'Stuur email wijzingsbereichten naar dit adres';
$lang['registernotify']        = 'Stuur informatie over nieuw aangemelde gebruikeers naar dit email adres';
$lang['mailfrom']              = 'Email adres voor automatische email';
$lang['gzip_output']           = 'Gebruik gzip Content-Encoding voor xhtml';
$lang['gdlib']                 = 'GD Lib versie';
$lang['im_convert']            = 'Path naar ImageMagick\'s convert tool';
$lang['jpg_quality']           = 'JPG compressie kwaliteit (0-100)';
$lang['subscribers']           = 'Page subscription ondersteuning aanzetten';
$lang['compress']              = 'Compacte CSS en javascript output';
$lang['hidepages']             = 'Verberg deze pagina\'s (regular expressions)';
$lang['send404']               = 'Stuur "HTTP 404/Page Not Found" voor niet bestaande pagina\'s';
$lang['sitemap']               = 'Genereer Google sitemap (dagen)';
$lang['broken_iua']            = 'Is de ignore_user_abort functie onbruikbaar op uw systeem? Dit kan een onbruikbare zoek index tot gevolg hebben. IIS+PHP/CGI staat hier bekend om. Zie <a href="http://bugs.splitbrain.org/?do=details&amp;task_id=852">Bug 852</a> voor meer informatie.';
$lang['xsendfile']             = 'Gebruik de X-Sendfile berichtkop om de webserver statische content te laten versturen? De webserver moet dit wel ondersteunen.';
$lang['xmlrpc']                = 'Inschakelen/uitschakelen XML-RPC interface.';
$lang['renderer_xhtml']        = 'Weergavesysteem om te gebruiken voor de hoofd (xhtml) wiki uitvoer';
$lang['renderer__core']        = '%s (dokuwiki core)';
$lang['renderer__plugin']      = '%s (plugin)';
$lang['rss_type']              = 'XML feed type';
$lang['rss_linkto']            = 'XML feed linkt naar';
$lang['rss_content']           = 'Wat moet er in de XML feed items weergegeven worden?';
$lang['rss_update']            = 'XML feed verversingsinterval (sec)';
$lang['recent_days']           = 'Hoeveel recente wijzigingen bewaren (days)';
$lang['rss_show_summary']      = 'XML feed samenvatting in titel weergeven';
$lang['target____wiki']        = 'Doelvenster voor interne links';
$lang['target____interwiki']   = 'Doelvenster voor interwiki links';
$lang['target____extern']      = 'Doelvenster voor externe  links';
$lang['target____media']       = 'Doelvenster voor media links';
$lang['target____windows']     = 'Doelvenster voor windows links';
$lang['proxy____host']         = 'Proxy server';
$lang['proxy____port']         = 'Proxy port';
$lang['proxy____user']         = 'Proxy gebruikersnaam';
$lang['proxy____pass']         = 'Proxy wachtwoord';
$lang['proxy____ssl']          = 'Gebruik SSL om een connectie te maken met de proxy';
$lang['safemodehack']          = 'Safemode hack aanzetten';
$lang['ftp____host']           = 'FTP server voor safemode hack';
$lang['ftp____port']           = 'FTP port voor safemode hack';
$lang['ftp____user']           = 'FTP gebruikersnaam voor safemode hack';
$lang['ftp____pass']           = 'FTP wachtwoord voor safemode hack';
$lang['ftp____root']           = 'FTP root directory voor safemode hack';
$lang['typography_o_0']        = 'geen';
$lang['typography_o_1']        = 'Alleen dubbele aanhalingstekens';
$lang['typography_o_2']        = 'Alle aanhalingstekens (functioneert mogelijk niet altijd)';
$lang['userewrite_o_0']        = 'geen';
$lang['userewrite_o_1']        = '.htaccess';
$lang['userewrite_o_2']        = 'DokuWiki intern';
$lang['deaccent_o_0']          = 'uit';
$lang['deaccent_o_1']          = 'accenten verwijderen';
$lang['deaccent_o_2']          = 'romaniseer';
$lang['gdlib_o_0']             = 'GD Lib niet beschikbaar';
$lang['gdlib_o_1']             = 'Version 1.x';
$lang['gdlib_o_2']             = 'Autodetectie';
$lang['rss_type_o_rss']        = 'RSS 0.91';
$lang['rss_type_o_rss1']       = 'RSS 1.0';
$lang['rss_type_o_rss2']       = 'RSS 2.0';
$lang['rss_type_o_atom']       = 'Atom 0.3';
$lang['rss_type_o_atom1']      = 'Atom 1.0';
$lang['rss_content_o_abstract'] = 'Abstract';
$lang['rss_content_o_diff']    = 'Geünificeerde Diff';
$lang['rss_content_o_htmldiff'] = 'HTML geformateerde diff tabel';
$lang['rss_content_o_html']    = 'Volledige HTML pagina inhoud';
$lang['rss_linkto_o_diff']     = 'verschillen';
$lang['rss_linkto_o_page']     = 'de gewijzigde pagina';
$lang['rss_linkto_o_rev']      = 'lijst van wijzigingen';
$lang['rss_linkto_o_current']  = 'de huidige pagina';
$lang['compression_o_0']       = 'geen';
$lang['compression_o_gz']      = 'gzip';
$lang['compression_o_bz2']     = 'bz2';
$lang['xsendfile_o_0']         = 'niet gebruiken';
$lang['xsendfile_o_1']         = 'Eigen lighttpd kop (voor release 1.5)';
$lang['xsendfile_o_2']         = 'Standaard X-Sendfile kop';
$lang['xsendfile_o_3']         = 'Propritary Nginx X-Accel-Redirect header';
