<?php
/** Arabic (العربية)
  *
  * @addtogroup Language
  */

$linkPrefixExtension = true;
$fallback8bitEncoding = 'windows-1256';

$rtl = true;
$defaultUserOptionOverrides = array(
	# Swap sidebar to right side by default
	'quickbar' => 2,
	# Underlines seriously harm legibility. Force off:
	'underline' => 0,
);

$namespaceNames = array(
	NS_MEDIA            => 'ملف',
	NS_SPECIAL          => 'خاص',
	NS_MAIN             => '',
	NS_TALK             => 'نقاش',
	NS_USER             => 'مستخدم',
	NS_USER_TALK        => 'نقاش_المستخدم',
	# NS_PROJECT set by $wgMetaNamespace
	NS_PROJECT_TALK     => 'نقاش' . '_$1',
	NS_IMAGE            => 'صورة',
	NS_IMAGE_TALK       => 'نقاش_الصورة',
	NS_MEDIAWIKI        => 'ميدياويكي',
	NS_MEDIAWIKI_TALK   => 'نقاش_ميدياويكي',
	NS_TEMPLATE         => 'قالب',
	NS_TEMPLATE_TALK    => 'نقاش_قالب',
	NS_HELP             => 'مساعدة',
	NS_HELP_TALK        => 'نقاش_المساعدة',
	NS_CATEGORY         => 'تصنيف',
	NS_CATEGORY_TALK    => 'نقاش_التصنيف'
);

$magicWords = array(
	'redirect'            => array( 0, "#تحويل", "#REDIRECT" ),
	'notoc'               => array( 0, "__لافهرس__", "__NOTOC__" ),
	'nogallery'           => array( 0, "__لامعرض__", "__NOGALLERY__" ),
	'forcetoc'            => array( 0, "__لصق_فهرس__", "__FORCETOC__" ),
	'toc'                 => array( 0, "__فهرس__", "__TOC__" ),
	'noeditsection'       => array( 0, "__لاتحريرقسم__", "__NOEDITSECTION__" ),
	'start'               => array( 0, "__ابدأ__", "__START__" ),
	'currentmonth'        => array( 1, "شهر", "شهر_حالي", "CURRENTMONTH" ),
	'currentmonthname'    => array( 1, "اسم_شهر", "اسم_شهر_حالي", "CURRENTMONTHNAME" ),
	'currentday'          => array( 1, "يوم", "CURRENTDAY" ),
	'currentday2'         => array( 1, "يوم2", "CURRENTDAY2" ),
	'currentdayname'      => array( 1, "اسم_يوم", "CURRENTDAYNAME" ),
	'currentyear'         => array( 1, "عام", "CURRENTYEAR" ),
	'currenttime'         => array( 1, "وقت", "CURRENTTIME" ),
	'currenthour'         => array( 1, "ساعة", "CURRENTHOUR" ),
	'localmonth'          => array( 1, "شهر_محلي", "LOCALMONTH" ),
	'localmonthname'      => array( 1, "اسم_شهر_محلي", "LOCALMONTHNAME" ),
	'localmonthabbrev'    => array( 1, "اختصار_شهر_محلي", "LOCALMONTHABBREV" ),
	'localday'            => array( 1, "يوم_محلي", "LOCALDAY" ),
	'localday2'           => array( 1, "يوم_محلي2", "LOCALDAY2" ),
	'localdayname'        => array( 1, "اسم_يوم_محلي", "LOCALDAYNAME" ),
	'localyear'           => array( 1, "عام_محلية", "LOCALYEAR" ),
	'localtime'           => array( 1, "وقت_محلي", "LOCALTIME" ),
	'localhour'           => array( 1, "ساعة_محلية", "LOCALHOUR" ),
	'numberofpages'       => array( 1, "عددالصفحات", "عدد_الصفحات", "NUMBEROFPAGES" ),
	'numberofarticles'    => array( 1, "عددالمقالات", "عدد_المقالات", "NUMBEROFARTICLES" ),
	'numberoffiles'       => array( 1, "عددالملفات", "عدد_الملفات", "NUMBEROFFILES" ),
	'numberofusers'       => array( 1, "عددالمستخدمين", "عدد_المستخدمين", "NUMBEROFUSERS" ),
	'pagename'            => array( 1, "اسم_صفحة", "PAGENAME" ),
	'pagenamee'           => array( 1, "عنوان_صفحة", "PAGENAMEE" ),
	'namespace'           => array( 1, "نطاق", "NAMESPACE" ),
	'namespacee'          => array( 1, "عنوان_نطاق", "NAMESPACEE" ),
	'talkspace'           => array( 1, "نطاق_نقاش,نطاق_النقاش", "TALKSPACE" ),
	'talkspacee'          => array( 1, "عنوان_نقاش,عنوان_النقاش", "TALKSPACEE" ),
	'fullpagename'        => array( 1, "اسم_كامل", "FULLPAGENAME" ),
	'fullpagenamee'       => array( 1, "عنوان_كامل", "FULLPAGENAMEE" ),
	'msg'                 => array( 0, "رسالة:", "MSG:" ),
	'subst'               => array( 0, "نسخ:", "نسخ_قالب:", "SUBST:" ),
	'msgnw'               => array( 0, "مصدر:", "مصدر_قالب:", "MSGNW:" ),
	'img_thumbnail'       => array( 1, "تصغير", "thumbnail", "thumb" ),
	'img_manualthumb'     => array( 1, "تصغير=$1", "thumbnail=$1", "thumb=$1" ),
	'img_right'           => array( 1, "يمين", "right" ),
	'img_left'            => array( 1, "يسار", "left" ),
	'img_none'            => array( 1, "بدون,بلا", "none" ),
	'img_width'           => array( 1, "$1بك", "$1px" ),
	'img_center'          => array( 1, "وسط", "center", "centre" ),
	'img_framed'          => array( 1, "إطار", "اطار", "framed", "enframed", "frame" ),
	'img_page'            => array( 1, "صفحة=$1", "صفحة $1", "page=$1", "page $1" ),
	'int'                 => array( 0, "محتوى:", "INT:" ),
	'sitename'            => array( 1, "اسم_الموقع", "SITENAME" ),
	'ns'                  => array( 0, "نط:", "NS:" ),
	'localurl'            => array( 0, "عنوان:", "LOCALURL:" ),
	'server'              => array( 0, "العنوان", "SERVER" ),
	'servername'          => array( 0, "اسم_عنوان", "SERVERNAME" ),
	'scriptpath'          => array( 0, "مسار", "SCRIPTPATH" ),
	'notitleconvert'      => array( 0, "لاتحويل_عنوان", "__NOTITLECONVERT__", "__NOTC__" ),
	'nocontentconvert'    => array( 0, "لاتحويل_محتوى", "__NOCONTENTCONVERT__", "__NOCC__" ),
	'currentweek'         => array( 1, "أسبوع", "CURRENTWEEK" ),
	'currentdow'          => array( 1, "رقم_يوم_أسبوع", "CURRENTDOW" ),
	'localweek'           => array( 1, "أسبوع_محلي", "LOCALWEEK" ),
	'revisionid'          => array( 1, "نسخة", "REVISIONID" ),
	'plural'              => array( 0, "جمع:", "PLURAL:" ),
	'fullurl'             => array( 0, "عنوان_كامل:", "FULLURL:" ),
	'newsectionlink'      => array( 1, "__رابط_قسم_جديد__", "__NEWSECTIONLINK__" ),
	'language'            => array( 0, "#لغة:", "#LANGUAGE:" ),
	'numberofadmins'      => array( 1, "عدد_إداريين", "عدد_الإداريين", "NUMBEROFADMINS" ),
	'special'             => array( 0, "خاص", "special" ),
);

$skinNames = array(
	'standard'    => array( "كلاسيك" ),
	'nostalgia'   => array( "نوستالجيا" ),
	'cologneblue' => array( "كولون بلو" ),
	'davinci'     => array( "دافنشي" ),
	'mono'        => array( "مونو" ),
	'monobook'    => array( "مونوبوك" ),
	'myskin'      => array( "واجهتي" ),
	'chick'       => array( "تشيك" ),
);

$digitTransformTable = array(
	'0' => '٠',
	'1' => '١',
	'2' => '٢',
	'3' => '٣',
	'4' => '٤',
	'5' => '٥',
	'6' => '٦',
	'7' => '٧',
	'8' => '٨',
	'9' => '٩',
	'.' => '٫', // wrong table?
	',' => '٬'
);

$specialPageAliases = array(
	'DoubleRedirects'           => array( "تحويلات_مزدوجة" ),
	'BrokenRedirects'           => array( "تحويلات_مكسورة" ),
	'Disambiguations'           => array( "توضيحات" ),
	'Userlogin'                 => array( "دخول" ),
	'Userlogout'                => array( "خروج" ),
	'Preferences'               => array( "تفضيلات" ),
	'Watchlist'                 => array( "المراقبة" ),
	'Recentchanges'             => array( "أحدث_التغييرات" ),
	'Upload'                    => array( "رفع" ),
	'Imagelist'                 => array( "قائمة_الصور" ),
	'Newimages'                 => array( "صور_جديدة" ),
	'Listusers'                 => array( "قائمة_المستخدمين" ),
	'Statistics'                => array( "احصاءات" ),
	'Randompage'                => array( "عشوائي", "صفحة_عشوائية" ),
	'Lonelypages'               => array( "صفحات_يتيمة" ),
	'Uncategorizedpages'        => array( "صفحات_غير_مصنفة" ),
	'Uncategorizedcategories'   => array( "تصنيفات_غير_مصنفة" ),
	'Uncategorizedimages'       => array( "صور_غير_مصنفة" ),
	'Unusedcategories'          => array( "تصنيفات_غير_مستخدمة" ),
	'Unusedimages'              => array( "صور_غير_مستخدمة" ),
	'Wantedpages'               => array( "صفحات_مطلوبة" ),
	'Wantedcategories'          => array( "تصنيفات_مطلوبة" ),
	'Mostlinked'                => array( "الأكثر_وصلا" ),
	'Mostlinkedcategories'      => array( "التصنيفات_الأكبر" ),
	'Mostcategories'            => array( "الصفحات_الأكثر_تصنيفا" ),
	'Mostimages'                => array( "الصور_الأكثر_وصلا" ),
	'Mostrevisions'             => array( "الأكثر_تعديلا" ),
	'Fewestrevisions'           => array( "الأقل_تعديلا" ),
	'Shortpages'                => array( "صفحات_قصيرة" ),
	'Longpages'                 => array( "صفحات_طويلة" ),
	'Newpages'                  => array( "صفحات_جديدة" ),
	'Ancientpages'              => array( "صفحات_قديمة" ),
	'Deadendpages'              => array( "صفحات_مسدودة" ),
	'Protectedpages'            => array( "صفحات_محمية" ),
	'Allpages'                  => array( "كل_الصفحات" ),
	'Prefixindex'               => array( "فهرس" ),
	'Ipblocklist'               => array( "قائمة_منع_أيبي" ),
	'Specialpages'              => array( "صفحات_خاصة" ),
	'Contributions'             => array( "مساهمات" ),
	'Emailuser'                 => array( "رسالة_إلكترونية" ),
	'Whatlinkshere'             => array( "ماذا_يصل" ),
	'Recentchangeslinked'       => array( "تغييرات_مرتبطة" ),
	'Movepage'                  => array( "نقل_صفحة" ),
	'Blockme'                   => array( "منعي" ),
	'Booksources'               => array( "مصدر_كتاب" ),
	'Categories'                => array( "تصنيفات" ),
	'Export'                    => array( "تصدير" ),
	'Version'                   => array( "النسخة" ),
	'Allmessages'               => array( "كل_الرسائل" ),
	'Log'                       => array( "سجل" ),
	'Blockip'                   => array( "منع_أيبي" ),
	'Undelete'                  => array( "استرجاع" ),
	'Import'                    => array( "استيراد" ),
	'Lockdb'                    => array( "غلق_قاعدة" ),
	'Unlockdb'                  => array( "فتح_قاعدة" ),
	'Userrights'                => array( "صلاحيات" ),
	'MIMEsearch'                => array( "بحث_MIME" ),
	'Unwatchedpages'            => array( "صفحات_غيرمراقبة" ),
	'Listredirects'             => array( "التحويلات" ),
	'Revisiondelete'            => array( "إلغاء_تعديل" ),
	'Unusedtemplates'           => array( "قوالب_غير_مستخدمة" ),
	'Randomredirect'            => array( "تحويل_عشوائي" ),
	'Mypage'                    => array( "صفحتي" ),
	'Mytalk'                    => array( "نقاشي" ),
	'Mycontributions'           => array( "مساهماتي" ),
	'Listadmins'                => array( "عرض_الإداريين" ),
	'Popularpages'              => array( "صفحات_مشهورة" ),
	'Search'                    => array( "بحث" ),
	'Resetpass'                 => array( "ضبط_كلمة_السر" ),
	'Withoutinterwiki'          => array( "بدون_إنترويكي" ),
);

$messages = array(
# User preference toggles
'tog-underline'               => 'خط تحت الوصلات',
'tog-highlightbroken'         => 'إظهار الوصلات المكسورة <a href="" class="new">بهذا الشكل</a> (البديل بهذا الشكل<a href="" class="internal">؟</a>).',
'tog-justify'                 => 'اجعل عرض الأسطر في الفقرات متساوٍ',
'tog-hideminor'               => 'خبئ التعديلات الطفيفة في أحدث التغييرات',
'tog-extendwatchlist'         => 'تمديد قائمة المراقبة لإظهار جميع التغيرات المطبقة',
'tog-usenewrc'                => 'شكل متقدم من صفحة أحدث التغييرات (يتطلب جافا سكريبت)',
'tog-numberheadings'          => 'رقّم الأقسام تلقائيًا',
'tog-showtoolbar'             => 'إظهار شريط التحرير (يتطلب جافا سكريبت)',
'tog-editondblclick'          => 'اذهب إلى صفحة التحرير عند النقر المزدوج على المقالة (يتطلب جافا سكريبت)',
'tog-editsection'             => 'تمكين تعديل الأقسام في المقال عن طريق وصلات [تحرير]',
'tog-editsectiononrightclick' => 'مكّن تحرير الأقسام في المقال عن طريق كبسة الفأرة اليمين على عناوين الأقسام (يتطلب جافا سكريبت)',
'tog-showtoc'                 => 'إظهار فهرس المحتويات (للمقالات التي تحتوي على أكثر من 3 أقسام)',
'tog-rememberpassword'        => 'تذكر عبر الجلسات',
'tog-editwidth'               => 'صندوق التحرير يأخذ كامل عرض المساحة المتاحة',
'tog-watchcreations'          => 'أضف الصفحات التي أنشأها إلى قائمة مراقبتي',
'tog-watchdefault'            => 'أضف الصفحات التي أقوم بتحريرها إلى قائمة مراقبتي',
'tog-watchmoves'              => 'أضف الصفحات التي أنقلها إلى قائمة مراقبتي',
'tog-watchdeletion'           => 'أضف الصفحات التي أمسحها إلى قائمة مراقبتي',
'tog-minordefault'            => "خيار ''هذا تعديل طفيف'' هو الخيار الافتراضي",
'tog-previewontop'            => 'إظهار العرض المسبق قبل صندوق التحرير',
'tog-previewonfirst'          => 'اعرض الصفحة عند حفظها بعد أول تعديل',
'tog-nocache'                 => 'امنع حفظ نسخة من الصفحة بعد زيارتها',
'tog-enotifwatchlistpages'    => 'أرسل لي رسالة بالتغييرات على الصفحة',
'tog-enotifusertalkpages'     => 'أرسل لي رسالة عندما يتم تحرير صفحة نقاشي',
'tog-enotifminoredits'        => 'أرسل لي رسالة للتغييرات الطفيفة أيضًا',
'tog-enotifrevealaddr'        => 'إظهار عنوان بريدي الإلكتروني في رسائل التنويه',
'tog-shownumberswatching'     => 'إظهار عدد المستخدمين المراقبين للصفحة',
'tog-fancysig'                => '<br />فقط توقيع (بدون وصلة أوتوماتيكية)',
'tog-externaleditor'          => 'استخدم محرر نصوص خارجي تلقائيًا',
'tog-externaldiff'            => 'استخدام برنامج خارجي لعرض الفروق',
'tog-showjumplinks'           => 'تفعيل وصلات "اذهب إلى".',
'tog-uselivepreview'          => 'استخدم الاستعراض السريع (جافاسكريبت) (تجريبي)',
'tog-forceeditsummary'        => 'نبهني عند تركي خانة "الملخص" فارغة',
'tog-watchlisthideown'        => 'أخف تعديلاتي من قائمة المراقبة',
'tog-watchlisthidebots'       => 'أخف تعديلات البوت من قائمة المراقبة',
'tog-watchlisthideminor'      => 'إخفاء التعديلات الطفيفة من قائمة مراقبتي',
'tog-nolangconversion'        => 'تعطيل تحويل المتغيرات',
'tog-ccmeonemails'            => 'إرسال نسخ من الرسائل التي أرسلها للمستخدمين الآخرين إلى بريدي الخاص',
'tog-diffonly'                => 'لا تعرض محتوى الصفحة عند المقارنة بين التعديلات.',

'underline-always'  => 'دائمًا',
'underline-never'   => 'لا',
'underline-default' => 'تبعًا لإعدادات المتصفح',

'skinpreview' => '(عرض)',

# Dates
'sunday'        => 'الأحد',
'monday'        => 'الاثنين',
'tuesday'       => 'الثلاثاء',
'wednesday'     => 'الأربعاء',
'thursday'      => 'الخميس',
'friday'        => 'الجمعة',
'saturday'      => 'السبت',
'sun'           => 'أحد',
'mon'           => 'اثنين',
'tue'           => 'ثلاثاء',
'wed'           => 'الأربعاء',
'thu'           => 'خميس',
'fri'           => 'جمعة',
'sat'           => 'سبت',
'january'       => 'يناير',
'february'      => 'فبراير',
'march'         => 'مارس',
'april'         => 'أبريل',
'may_long'      => 'مايو',
'june'          => 'يونيو',
'july'          => 'يوليو',
'august'        => 'أغسطس',
'september'     => 'سبتمبر',
'october'       => 'أكتوبر',
'november'      => 'نوفمبر',
'december'      => 'ديسمبر',
'january-gen'   => 'يناير',
'february-gen'  => 'فبراير',
'march-gen'     => 'مارس',
'april-gen'     => 'أبريل',
'may-gen'       => 'مايو',
'june-gen'      => 'يونيو',
'july-gen'      => 'يوليو',
'august-gen'    => 'أغسطس',
'september-gen' => 'سبتمبر',
'october-gen'   => 'أكتوبر',
'november-gen'  => 'نوفمبر',
'december-gen'  => 'ديسمبر',
'jan'           => 'يناير',
'feb'           => 'فبراير',
'mar'           => 'مارس',
'apr'           => 'أبريل',
'may'           => 'مايو',
'jun'           => 'يونيو',
'jul'           => 'يوليو',
'aug'           => 'أغسطس',
'sep'           => 'سبتمبر',
'oct'           => 'أكتوبر',
'nov'           => 'نوفمبر',
'dec'           => 'ديسمبر',

# Bits of text used by many pages
'categories'            => '{{PLURAL:$1|تصنيف الصفحة|تصنيفات الصفحة}}',
'pagecategories'        => '{{PLURAL:$1|تصنيف الصفحة|تصنيفات الصفحة}}',
'category_header'       => 'المقالات في التصنيف "$1"',
'subcategories'         => 'التصنيفات الفرعية',
'category-media-header' => 'الملفات في التصنيف "$1"',

'mainpagetext'      => "<big>'''تم تشغيل ميدياويكي بنجاح.'''</big>",
'mainpagedocfooter' => 'أنظر [http://meta.wikimedia.org/wiki/Help:Contents دليل المستخدم] لمعلومات حول استخدام الويكي.

== البداية ==

* [http://www.mediawiki.org/wiki/Help:Configuration_settings قائمة إعدادات الضبط]
* [http://www.mediawiki.org/wiki/Help:FAQ اسئلة متكررة حول ميدياويكي]
* [http://mail.wikimedia.org/mailman/listinfo/mediawiki-announce القائمة البريدية الخاصة باصدار ميدياويكي]',

'about'          => 'حول',
'article'        => 'مقالة',
'newwindow'      => '(يفتح في شباك جديد)',
'cancel'         => 'الغ',
'qbfind'         => 'بحث',
'qbbrowse'       => 'تصفح',
'qbedit'         => 'تحرير',
'qbpageoptions'  => 'هذه الصفحة',
'qbpageinfo'     => 'سياق النص',
'qbmyoptions'    => 'صفحاتي',
'qbspecialpages' => 'الصفحات الخاصّة',
'moredotdotdot'  => 'مزيد...',
'mypage'         => 'صفحتي',
'mytalk'         => 'صفحة نقاشي',
'anontalk'       => 'تناقش مع عنوان الأيبي هذا',
'navigation'     => 'تصفح',

# Metadata in edit box
'metadata_help' => 'معطيات ميتا:',

'errorpagetitle'    => 'خطأ',
'returnto'          => 'الرجوع إلى $1.',
'tagline'           => 'من {{SITENAME}}',
'help'              => 'مساعدة',
'search'            => 'بحث',
'searchbutton'      => 'بحث',
'go'                => 'إذهب',
'searcharticle'     => 'إذهب',
'history'           => 'التاريخ',
'history_short'     => 'التاريخ',
'updatedmarker'     => 'تم تعديل الصفحة بعد آخر زيارة',
'info_short'        => 'معلومات',
'printableversion'  => 'نسخة للطباعة',
'permalink'         => 'وصلة دائمة',
'print'             => 'طباعة',
'edit'              => 'تعديل',
'editthispage'      => 'عدل هذه الصفحة',
'delete'            => 'حذف',
'deletethispage'    => 'حذف هذه الصفحة',
'undelete_short'    => 'استرجاع {{PLURAL:$1|تعديل واحد|تعديلان|$1 تعديلات|$1 تعديل|$1 تعديلا}}',
'protect'           => 'حماية',
'protect_change'    => 'تغيير مستوى الحماية',
'protectthispage'   => 'حماية هذه الصفحة',
'unprotect'         => 'إزالة الحماية',
'unprotectthispage' => 'إزالة الحماية عن الصفحة',
'newpage'           => 'صفحة جديدة',
'talkpage'          => 'ناقش هذه الصفحة',
'talkpagelinktext'  => 'النقاش',
'specialpage'       => 'صفحة خاصة',
'personaltools'     => 'أدوات شخصية',
'postcomment'       => 'أرسل تعليق',
'articlepage'       => 'عرض المقالة',
'talk'              => 'النقاش',
'views'             => 'معاينة',
'toolbox'           => 'أدوات',
'userpage'          => 'عرض صفحة المستخدم',
'projectpage'       => 'عرض الصفحة العامة',
'imagepage'         => 'عرض صفحة الصورة',
'mediawikipage'     => 'عرض صفحة الرسالة',
'templatepage'      => 'عرض صفحة القالب',
'viewhelppage'      => 'عرض صفحة المساعدة',
'categorypage'      => 'عرض صفحة التصنيف',
'viewtalkpage'      => 'عرض النقاش',
'otherlanguages'    => ' لغات أخرى',
'redirectedfrom'    => '(تم التحويل من $1)',
'redirectpagesub'   => 'صفحة تحويل',
'lastmodifiedat'    => 'آخر تعديل لهذه الصفحة كان في $2, $1.', # $1 date, $2 time
'viewcount'         => 'تم عرض هذه الصفحة {{plural:$1|مرة واحدة|مرتان|$1 مرات|$1 مرة}}.',
'protectedpage'     => 'صفحة محمية',
'jumpto'            => 'اذهب إلى:',
'jumptonavigation'  => 'تصفح',
'jumptosearch'      => 'بحث',

# All link text and link target definitions of links into project namespace that get used by other message strings, with the exception of user group pages (see grouppage) and the disambiguation template definition (see disambiguations).
'aboutsite'         => 'حول {{SITENAME}}',
'aboutpage'         => 'Project:حول',
'bugreports'        => 'تقارير الأخطاء',
'bugreportspage'    => 'Project:بلاغ_أخطاء',
'copyright'         => 'المحتويات تحت  $1.',
'copyrightpagename' => 'حقوق النسخ في {{SITENAME}}',
'copyrightpage'     => 'Project:حقوق النسخ',
'currentevents'     => 'الأحداث الجارية',
'currentevents-url' => 'الأحداث الجارية',
'disclaimers'       => 'عدم مسؤولية',
'disclaimerpage'    => 'Project:عدم_مسؤولية_عام',
'edithelp'          => 'مساعدة التحرير',
'edithelppage'      => 'Project:مساعدة التحرير',
'faq'               => 'الأسئلة الأكثر تكرارا',
'faqpage'           => 'Project:أسئلة متكررة',
'helppage'          => 'Help:محتويات',
'mainpage'          => 'الصفحة الرئيسية',
'policy-url'        => '{{ns:project}}:سياسة',
'portal'            => 'بوابة المجتمع',
'portal-url'        => 'Project:بوابة المجتمع',
'privacy'           => 'سياسة الخصوصية',
'privacypage'       => 'Project:سياسة الخصوصية',
'sitesupport'       => 'التبرعات',
'sitesupport-url'   => 'Project:دعم الموقع',

'badaccess'        => 'خطأ في الصلاحيات المطلوبة',
'badaccess-group0' => 'ليس من المسموح لك تنفيذ ما طلبت.',
'badaccess-group1' => 'العمل الذي طلبتموه ينحصر في مجموعه مستخدمي $1.',
'badaccess-group2' => 'العمل طلبتموه محدود للمستخدمين في احدى المجموعات $1 .',
'badaccess-groups' => 'الأمر الذي طلبته مسموح به فقط من خلال أحد الأعضاء في المجموعات $1.',

'versionrequired'     => 'يلزم الإصدار $1 من ميدياويكي',
'versionrequiredtext' => 'يلزم الإصدار رقم $1 من ميدياويكي لاستعمال هذه الصفحة. انظر [[Special:Version|رقم الإصدار]]',

'ok'                  => 'موافق',
'pagetitle'           => '$1 - {{SITENAME}}',
'retrievedfrom'       => 'تمّ الاسترجاع من "$1"',
'youhavenewmessages'  => 'يوجد لديك $1 ($2).',
'newmessageslink'     => 'رسائل جديدة',
'newmessagesdifflink' => 'آخر تغيير',
'editsection'         => 'تحرير',
'editold'             => 'تحرير',
'editsectionhint'     => 'تحرير القسم: $1',
'toc'                 => 'فهرست',
'showtoc'             => 'إظهار',
'hidetoc'             => 'إخفاء',
'thisisdeleted'       => 'هل تريد استعراض أو استرجاع $1؟',
'viewdeleted'         => 'عرض $1؟',
'restorelink'         => '{{PLURAL:$1|تعديل واحد محذوف|تعديلان محذوفان|$1 تعديلات محذوفة|$1 تعديل محذوف|$1 تعديلا محذوفا}}',
'feedlinks'           => 'تلقيم:',
'feed-invalid'        => 'نوع إشتراك التليقم خاطئ.',

# Short words for each namespace, by default used in the 'article' tab in monobook
'nstab-main'      => 'مقالة',
'nstab-user'      => 'صفحة مستخدم',
'nstab-media'     => 'صفحة ميديا',
'nstab-special'   => 'خاص',
'nstab-project'   => 'حول',
'nstab-image'     => 'ملف',
'nstab-mediawiki' => 'رسالة',
'nstab-template'  => 'قالب',
'nstab-help'      => 'مساعدة',
'nstab-category'  => 'تصنيف',

# Main script and global functions
'nosuchaction'      => 'لا يوجد أمر كهذا',
'nosuchactiontext'  => 'الأمر في مسار الصفحة لم يتم التعرف عليه من خلال الويكي',
'nosuchspecialpage' => 'لا توجد صفحة خاصة بهذا الاسم',
'nospecialpagetext' => 'لقد طلبت صفحة خاصة لا يمكن التعرف عليها من قبل نظام الويكي.',

# General errors
'error'                => 'خطأ',
'databaseerror'        => 'خطأ في قاعدة البيانات',
'dberrortext'          => 'حدث خطأ في صيغة الاستعلام.
ربما يكون هذا عيب بالبرنامج.
آخر استعلام طُلب من قاعدة البيانات كان:
<blockquote><tt>$1</tt></blockquote>
من داخل الدالة "<tt>$2</tt>".
MySQL أرجعت الخطأ "<tt>$3: $4</tt>".',
'dberrortextcl'        => 'حدث خطأ في صيغة الاستعلام.
آخر استعلام طُلب من قاعدة البيانات كان:
"$1"
من داخل الدالة "$2".
MySQL أرجعت الخطأ "$3: $4"',
'noconnect'            => 'عفوا! تعاني الويكي من بعض المشاكل التقنية, و لا يمكن الاتصال بخادم قاعدة البيانات. <br />
$1',
'nodb'                 => 'لم يتمكن من اختيار قاعدة البيانات $1',
'cachederror'          => 'هذه نسخة مخبأة من الصفحة المطلوبة, و قد لا تكون محدثة.',
'laggedslavemode'      => 'تحذير: هذه الصفحة قد لا تحتوي على أحدث التغييرات.',
'readonly'             => 'قاعدة البيانات مغلقة',
'enterlockreason'      => 'اذكر سبب المنع والوقت المتوقع لإزالة المنع',
'readonlytext'         => 'قاعدة البيانات مغلقة حاليا أمام الإضافات والتعديلات، السبب غالبا ما يكون الصيانة، وستعود قاعدة البيانات للوضع الطبيعي قريبا.
عندما تم أغلاق قاعدة البيانات أمام التعديلات والإضافات تم أعطاء السبب التالي:
<p>$1',
'missingarticle'       => 'لم تعثر قاعدة البيانات على نص الصفحة المفترض، باسم "$1".

عادة ما يكون السبب في ذلك اتّباع صفحة فروق قديمة أو وصلة لتاريخ صفحة محذوفة. إذا كنت تعتقد أن السبب غير ذلك، فمن الممكن أن يكون هنالك علة في البرنامج. الرجاء إعلام الإداريين عن هذا الخطأ مع ذكر الوصلة.',
'internalerror'        => 'خطأ داخلي',
'filecopyerror'        => 'لا يمكن نسخ الملف من  "$1" إلى "$2".',
'filerenameerror'      => 'لا يمكن تغيير اسم الملف من  "$1" إلى "$2".',
'filedeleteerror'      => 'لا يمكن حذف الملف "$1".',
'filenotfound'         => 'لا يمكن إيجاد الملف "$1".',
'unexpected'           => 'قيمة غير متوقعة: "$1"="$2".',
'formerror'            => 'خلل: طلبك لم ينفذ',
'badarticleerror'      => 'لا يمكن إجراء هذا الطلب على هذه الصفحة.',
'cannotdelete'         => 'لم يتم مسح الصفحة أو الملف المطلوب. ( ربما يكون قد تم مسحه من قبل مستخدم آخر. )',
'badtitle'             => 'عنوان خاطئ',
'badtitletext'         => 'عنوان الصفحة المطلوبة إما خاطئ أو فارغ، و ربما الوصلة بين اللغات أو بين المشاريع خاطئة. ومن الممكن وجود رموز لا تصلح للاستعمال في العنوان.',
'perfdisabled'         => 'عفوا! هذه الخاصية معطله حاليا لإنها تؤثر بشكل سلبي على أداء الويكي و تبطئها إلى حد استحالة استخدامها.',
'perfdisabledsub'      => 'هذه نسخة محفوظة مسبقا من $1:', # obsolete?
'perfcached'           => 'القائمة التالية تمّ تخزينها في وقت سابق وقد لا تكون محدّثة بعد:',
'perfcachedts'         => 'البيانات التالية مخزنة سابقا، آخر تحديث لها كان في $1.',
'querypage-no-updates' => 'تم تعطيل عمليات التحديث والإضافة لهذه الصفحة في الوقت الحالي. البيانات المعروضة هنا لن يتم تحديثها الأن.',
'wrong_wfQuery_params' => 'معاملات خاطئة في wfQuery()<br />
الدالة: $1<br />
الاستعلام: $2',
'viewsource'           => 'عرض المصدر للمقالة',
'viewsourcefor'        => 'ل $1',
'protectedpagetext'    => "'''تمت حماية هذه الصفحة لمنع التعديلات.'''",
'viewsourcetext'       => 'يمكنك مشاهدة ونسخ مصدر هذه الصفحة:',
'protectedinterface'   => 'هذه الصفحة تتضمن نص الواجهة للبرنامج ولقد تم إغلاقها لمنع التخريب.',
'editinginterface'     => "'''تحذير''': أنت تقوم بتحرير صفحة تستخدم في الواجهة النصية للبرنامج. أية تغييرات لهذه الصفحة سوف تظهر في الواجهة لجميع المستخدمين الآخرين.",
'sqlhidden'            => '(استعلام SQL مخبئ)',
'cascadeprotected'     => 'تمت حماية هذه الصفحة من التعديل بسبب كونها مدمجة في الصفحات التالية، والتي تم استعمال خاصية "حماية الصفحات المدمجة" بها:',

# Login and logout pages
'logouttitle'                => 'خروج المستخدم',
'logouttext'                 => 'أنت الآن غير مسجل الدخول للنظام.
تستطيع المتابعة باستعمال {{SITENAME}} كمجهول، أو الدخول مرة أخرى بنفس الاسم أو باسم آخر. من الممكن أن ترى بعض الصفحات في كما وأنك مسجل في النظام.، وذلك بسبب استعمال الصفحات المخبأة في المتصفح لديك.',
'welcomecreation'            => '== أهلا بك يا $1! ==
تم إنشاء حسابك بنجاح. لا تنسى أن تقوم بتغيير وتحديد تفضيلاتك في {{SITENAME}}.',
'loginpagetitle'             => 'دخول المستخدم',
'yourname'                   => 'اسم المستخدم',
'yourpassword'               => 'كلمة السر',
'yourpasswordagain'          => 'أعد كتابة كلمة السر',
'remembermypassword'         => 'تذكر كلمة السر عبر الجلسات.',
'yourdomainname'             => 'النطاق الخاص بك',
'externaldberror'            => 'هناك إما خطأ في دخول قاعدة البيانات الخارجية أو أنه غير مسموح لك بتحديث حسابك الخارجي.',
'loginproblem'               => '<b>حدثت مشكلة أثناء الدخول.</b><br />يرجى المحاولة مرى أخرى!',
'alreadyloggedin'            => '<strong>$1، أنت مسجل للدخول من قبل!</strong><br />',
'login'                      => 'دخول',
'loginprompt'                => 'يجب أن يدعم متصفحك الكوكيز لتتمكن من الدخول.',
'userlogin'                  => 'دخول / إنشاء حساب',
'logout'                     => 'خروج',
'userlogout'                 => 'خروج',
'notloggedin'                => 'غير مسجل',
'nologin'                    => 'لم تشترك بعد؟ $1.',
'nologinlink'                => 'قم بإنشاء حساب',
'createaccount'              => 'أنشئ حسابا جديدا',
'gotaccount'                 => 'هل قمت بالتسجيل سابقا؟ $1.',
'gotaccountlink'             => 'دخول',
'createaccountmail'          => 'عبر البريد الإلكتروني',
'badretype'                  => 'كلمات السر التي أدخلتها غير متطابقة.',
'userexists'                 => 'اسم المستخدم الذي إخترته مستخدم من قبل، يرجى إختيار اسم مستخدم آخر.',
'youremail'                  => 'بريدك الإلكتروني*',
'username'                   => 'اسم المستخدم:',
'uid'                        => 'رقم المستخدم:',
'yourrealname'               => 'اسمك الحقيقي*',
'yourlanguage'               => 'اللغة',
'yourvariant'                => 'مختلف',
'yournick'                   => 'اللقب الخاص بك (للتواقيع)',
'badsig'                     => 'صيغة توقيع خاطئة؛ تحقق من بيانات الHTML.',
'email'                      => 'البريد الإلكتروني',
'prefs-help-email-enotif'    => 'يستخدم هذا العنوان أيضا في إرسال تنبيهات البريد الإلكتروني إليك لو قمت بتفعيل تلك الخيارات.',
'prefs-help-realname'        => '* الاسم الحقيقي (اختياري): لو اخترت أن تعرض اسمك هنا, سيستخدم هذا في الإشارة إلى مساهماتك.',
'loginerror'                 => 'خطأ في الدخول',
'prefs-help-email'           => '* البريد الإلكتروني هو مدخل اختياري يمكّن الآخرين من الاتصال بك عن طريق صفحتك أو صفحة نقاشك من دون معرفة عنوانك.',
'nocookiesnew'               => 'تم إنشاء حساب المستخدم و لكنك لست مسجل الدخول بعد. يستخدم {{SITENAME}} كوكيز لتسجيل الدخول. لديك الكوكيز معطلة. من فضلك فعلها, ثم أدخل باسم الحساب و كلمة السر الجديدتين.',
'nocookieslogin'             => '{{SITENAME}} يستخدم الكوكيز لتسجيل الدخول. لديك الكوكيز معطلة. من فضلك فعلها, ثم أدخل باسم الحساب و كلمة السر الجديدتين.
n.',
'noname'                     => 'لم تحدد اسم مستخدم صحيح.',
'loginsuccesstitle'          => 'تم الدخول بشكل صحيح',
'loginsuccess'               => 'لقد قمت بتسجيل الدخول ل{{SITENAME}} باسم "$1".',
'nosuchuser'                 => 'لا يوجد مستخدم بالاسم "$1".
تأكد من إملاء الاسم، أو استعمل النموذج الموجود في الأسفل لإنشاء مستخدم جديد.',
'nosuchusershort'            => 'لا يوجد مستخدم باسم "$1"، تأكد من كتابة الاسم الذي تريده بالطريقة الصحيحة.',
'nouserspecified'            => 'يجب عليك تحديد اسم المستخدم',
'wrongpassword'              => 'كلمة السر التي أدخلتها غير صحيحة، يرجى إعادة المحاولة.',
'wrongpasswordempty'         => 'لقد أدخلت كلمة سر فارغة. حاول مرة أخرى.',
'mailmypassword'             => 'أرسل لي كلمة السر عبر البريد الإلكتروني.',
'passwordremindertitle'      => 'تذكير بكلمة السر من {{SITENAME}}',
'passwordremindertext'       => 'لقد طلب شخص ما (غالبًا أنت، من عنوان الأيبي $1) أن نرسل لك كلمة سر جديدة لـ{{SITENAME}} ($4).

كلمة السرّ الآن للمستخدم "$2" هي "$3". 
عليك أن تقوم بالدخول إلى الموقع وتغيير كلمة السر خاصتك الآن إلا إذا لم تكن أنت من طلب هذا، اقرأ أدناه في هذه الحالة.

إذا لم تكن أنت من قام بطلب كلمة السرّ أو أنك تذكرّت كلمة السرّ السابقة ولا ترغب بتغييرها فبإمكانك أن تتغاضى عن هذه الرسالة وأن تستمر في استخدام كلمة السرّ السابقة خاصتك.',
'noemail'                    => 'لا يوجد أي عنوان بريدي مسجل للمستخدم "$1".',
'passwordsent'               => 'تم إرسال كلمة سر جديدة إلى العنوان البريدي المسجل للمستخدم "$1".
يرجى محاولة تسجيل الدخول مرة أخرى عند استلامها.',
'blocked-mailpassword'       => 'تم منع العنوان الخاص بك من التحرير، ولمنع التخريب لا يمكنك أن تستخدم خاصية استرجاع كلمة السر.',
'eauthentsent'               => 'تم إرسال رسالة إلكترونية إلى العنوان المسمى. حتى ترسل أي رسالة أخرى لذلك الحساب عليك أن تتبع التعليمات الواردة في الرسالة لتأكيد أن هذا العنوان هو لك بالفعل.',
'throttled-mailpassword'     => 'تم بالفعل إرسال تذكير بكلمة السر، في خلال الـ$1 ساعة الماضية.
لمنع التخريب، تذكير واحد فقط سيتم إرساله كل
$1 ساعة.',
'mailerror'                  => 'خطأ أثناء إرسال البريد: $1',
'acct_creation_throttle_hit' => 'معذرة، لقد قمت بإنشاء $1 حساب. لا يمكنك عمل المزيد.',
'emailauthenticated'         => 'تم تأكيد بريدك الإلكتروني على $1.',
'emailnotauthenticated'      => '<strong>لم يتمّ التحقق</strong> من بريدك الإلكتروني. لن يتمّ إرسال رسائل لأي من الميزات التالية.',
'noemailprefs'               => '<strong>يجب إعطاء بريد إلكتروني لتفعيل هذه الخصائص.</strong>',
'emailconfirmlink'           => 'أكد بريدك الإلكتروني',
'invalidemailaddress'        => 'لا يمكن قبول العنوان الإلكتروني الذي زودته حيث يبدو خاطئا. من فضلك ضع عنوانا مضبوطا أو أفرغ هذا الحقل.',
'accountcreated'             => 'تم إنشاء الحساب',
'accountcreatedtext'         => 'تم إنشاء الحساب الخاص ب$1.',

# Password reset dialog
'resetpass'               => 'إعادة ضبط كلمة سر الحساب',
'resetpass_announce'      => 'تم تسجيل دخولك بكلمة سر مؤقتة. للدخول بشكل نهائي، يجب عليك ضبط كلمة سر جديدة هنا:',
'resetpass_text'          => '<!-- أضف نصا هنا -->',
'resetpass_header'        => 'إعادة ضبط كلمة السر',
'resetpass_submit'        => 'ضبط كلمة السر والدخول',
'resetpass_success'       => 'تم تغيير كلمة السر بنجاخ! يتم تسجيل دخولك الأن...',
'resetpass_bad_temporary' => 'كلمة السر المؤقتة خاطئة. ربما تم تغيير كلمة السر فعليا بنجاح أو ربما قمت بطلب كلمة سر مؤقتة جديدة.',
'resetpass_forbidden'     => 'لا يمكن تغيير كلمات السر على هذه الويكي',
'resetpass_missing'       => 'لم يتم استقبال بيانات',

# Edit page toolbar
'bold_sample'     => 'نص عريض',
'bold_tip'        => 'نص عريض',
'italic_sample'   => 'نص مائل',
'italic_tip'      => 'نص مائل',
'link_sample'     => 'عنوان وصلة',
'link_tip'        => 'وصلة داخلية',
'extlink_sample'  => 'http://www.example.com نص الوصلة',
'extlink_tip'     => 'وصلة خارجية (تذكر إضافة http:// قبل عنوان الوصلة)',
'headline_sample' => 'نص عنوان رئيسي',
'headline_tip'    => 'عنوان من المستوى الثاني',
'math_sample'     => 'أدخل الصيغة هنا',
'math_tip'        => 'صيغة رياضية',
'nowiki_sample'   => 'أدخل النص غير المنسق هنا',
'nowiki_tip'      => 'أهمل تهيئة الويكي',
'image_sample'    => 'اسم الصورة|تصغير|تعليق',
'image_tip'       => 'إدراج صورة',
'media_tip'       => 'وصلة ملف وسائط',
'sig_tip'         => 'توقيعك من الساعة والتاريخ',
'hr_tip'          => 'خط أفقي (تجنب الاستخدام بكثرة)',

# Edit pages
'summary'                   => 'ملخص',
'subject'                   => 'موضوع',
'minoredit'                 => 'هذا تعديل طفيف',
'watchthis'                 => 'راقب هذه الصفحة',
'savearticle'               => 'حفظ الصفحة',
'preview'                   => 'عرض مسبق',
'showpreview'               => 'عرض التعديلات',
'showlivepreview'           => 'عرض مباشر',
'showdiff'                  => 'أظهر الفرق',
'anoneditwarning'           => "'''تحذير:''' لم تقم بالدخول. سيتم تسجيل عنوان الأيبي الخاص بك في تاريخ هذه الصفحة.",
'missingsummary'            => "'''تنبيه:''' لم تقم بكتابة ملخص للتعديل الذي قمت به. إذا قمت باختيار «حفظ» مرة أخرى فسوف يحفظ تغييرك بلا ملخص.",
'missingcommenttext'        => 'الرجاء كتابة تعليق في الأسفل.',
'missingcommentheader'      => "'''تنبيه:''' لم تقم بوضع عنوان لهذا التعليق، إذا قمت بالضغط على ''حفظ الصفحة'' مجددا، سيتم حفظ تعليقك بدون عنوان.",
'summary-preview'           => 'عرض مسبق للملخص',
'subject-preview'           => 'معاينة العنوان',
'blockedtitle'              => 'المستخدم ممنوع',
'blockedtext'               => "<big>'''تم منع اسم المستخدم أو عنوان الأيبي الخاص بك.'''</big>

سبب المنع هو: ''$2''. وقام بالمنع $1.

من الممكن التواصل مع $1 للنقاش حول المنع، أو مع أحد [[{{MediaWiki:grouppage-sysop}}|الإداريين]] حول ذلك>
تذكر أنه لا يمكن لك استعمال خاصية إرسال رسائل إلكترونية للمستخدمين إلا إذا كنت قد وضعت عنوان بريدي صحيح في صفحة [[Special:Preferences|التفضيلات]] الخاصة بك.

عنوان ال IP الخاص بك حاليا هو $3 وكود المنع هو #$5. يرجى إضافة أيهما أو كلاهما في أي رسالة للتساؤل حول المنع.",
'blockedoriginalsource'     => "مصدر '''$1''' ظاهر هنا:",
'blockededitsource'         => "مبين بالأسفل نص '''تعديلك''' في '''$1''':",
'whitelistedittitle'        => 'الولوج ضروري للتحرير',
'whitelistedittext'         => 'يجب عليك $1 لتتمكن من تعديل الصفحات.',
'whitelistreadtitle'        => 'الولوج ضروري للقراءة',
'whitelistreadtext'         => 'يتعين عليك [[Special:Userlogin|الولوج]] لتتمكن من قراءة الصفحات.',
'whitelistacctitle'         => 'لا يسمح لك بإنشاء إشتراك',
'whitelistacctext'          => 'حتى يُسمح لك بإنشاء حسابات في هذه الويكي عليك أن تقوم [[Special:Userlogin|بالدخول]] وأن تمتلك الصلاحيات المناسبة.',
'confirmedittitle'          => 'تأكيد البريد الإلكتروني مطلوب لبدء التعديل',
'confirmedittext'           => 'يجب عليك تأكيد بريدك الإلكتروني قبل التعديل. من فضلك اكتب و أكد بريدك الإلكتروني من خلال [[Special:Preferences|تفضيلاتك]]',
'nosuchsectiontitle'        => 'لا يوجد قسم بهذا الاسم',
'nosuchsectiontext'         => 'لقد حاولت تحرير قسما غير موجودا. وحيث أنه لا يوجد القسم $1، فلا يمكن حفظ تعديلك.',
'loginreqtitle'             => 'مطلوب الدخول أولا.',
'loginreqlink'              => 'دخول',
'loginreqpagetext'          => 'يجب عليك $1 لتشاهد صفحات أخرى.',
'accmailtitle'              => 'تم إرسال كلمة السر.',
'accmailtext'               => "تم إرسال كلمة السر الخاصة بـ '$1' إلى العنوان $2.",
'newarticle'                => '(جديد)',
'newarticletext'            => "لقد تبعت وصلة لصفحة لم يتم إنشائها بعد.
لإنشاء هذه الصفحة إبدأ بالكتابة في الصندوق بالأسفل.
(أنظر في [[{{MediaWiki:helppage}}|صفحة المساعدة]] للمزيد من المعلومات)
إذا كانت زيارتك لهذه الصفحة بالخطأ، إضغم على زر ''رجوع'' في متصفح الإنترنت لديك.",
'anontalkpagetext'          => '----
هذه صفحة نقاش لمستخدم مجهول، وهو المستخدم الذي لم يقم بإنشاء حساب في {{SITENAME}}، أو لا يستعمل ذلك الحساب.
لذا يتم استعمال رقم ال IP للتعريف به. من الممكن أن يشترك عدد من المستخدمين بنفس رقم ال IP. إذا كنت مستخدم مجهول
وترى أن رسائل خير موجهة لك قد وصلتك، من الممكن أن تقوم [[Special:Userlogin|بإنشاء حساب أو القيام بالدخول]]
حتى يزول الخلط بينك وبين المستخدمين المجهولين الآخرين.',
'noarticletext'             => '(لا يوجد حاليا أي نص في هذه الصفحة)',
'clearyourcache'            => "ملاحظة: قم بإعادة تحميل الصفحة لرؤية التغييرات. إذا لم تظهر التغييرات بعد إعادة التحميل، قد يكون متصفح الإنترنت الذي تستخدمه يقوم بعرض نسخة مخزنة سابقًا. لتجنب عرض هذه النسخة، قد يلزم أن تقوم بأحد هذه الخطوات:
* في متصفح '''موزيلا''' أو '''فايرفوكس''' أو '''سفاري''' اضغظ على ''مفتاح Shift'' أثناء قيامك بضغط زر إعادة التحميل (''Refresh'') أو اضغط ''Ctrl-Shift-R'' (''Cmd-Shift-R'' في حاسب أبل).
* في متصفح '''إنترنت إكسبلورر''' اضغط على مفتاح ''Ctrl'' أثناء قيامك بالنقر على زر إعادة التحميل (''Refresh'') أو اضغط ''Ctrl-F5''.
* في متصفح''' كونكيرر''' (Konqueror) انقر على زر إعادة التحميل (''Reload'') أو اضغط على مفتاح '''F5'''.
* في متصفح '''أوبرا''' (Opera) قد يكون من الضروري مسح المحتويات المخزنة عن طريق قائمة التفضيلات (''Tools&rarr;Preferences'').",
'usercssjsyoucanpreview'    => "<strong>ملاحظة:</strong> استعمل زرّ 'عرض التعديلات' لتجربة النمط (CSS) أو الجافا سكريبت الجديد قبل حفظ الصفحة.",
'usercsspreview'            => "'''تذكر أنك تقوم بعرض الأنماط المتراصة (CSS) فقط وأنك لم تقم بحفظها بعد!'''",
'userjspreview'             => "'''تذكر أنك فقط تجرب/تعرض الجافا سكريبت الخاص بك, لم يتم الحفظ بعد!'''",
'userinvalidcssjstitle'     => "'''تحذير:''' لا توجد واجهة  \"\$1\". تذكر أن ملفات ال.css و ال.js تستخدم حروف صغيرة في العنوان , كمثال User:Foo/monobook.css و ليس User:Foo/Monobook.css.",
'updated'                   => '(محدثة)',
'note'                      => '<strong>ملاحظة:</strong>',
'previewnote'               => 'تذكر، هذا فقط عرض مسبق للصفحة، ولم يتم حفظه بعد!',
'previewconflict'           => 'العرض يوضح النص الموجود في صندوق التحرير العلوي و الذي سيظهر إذا اخترت الحفظ.',
'session_fail_preview'      => '<strong>عفوًا! لم نتمكن من حفظ التعديلات التي قمت بها نتيجة لضياع في المعلومات في هذه الجلسة. الرجاء المحاولة مرة أخرى. في حال استمرار المشكلة حاول أن تقوم بالخروج ومن ثم الدخول مرة أخرى.</strong>',
'session_fail_preview_html' => "<strong>عذرًا ! لا نستطيع تسجيل تعديلاتك بسبب فقدان معلومات حول الجلسات.</strong>

''بسبب أن wiki فعل HTML , تم إخفاء عرض التعديلات كوقاية و حماية  ضد الهجمات ب Javascript.''

<strong>إدا كانت هذه محاولة تعديل صادقة, حاول مرة أخرى. و في حالة الفشل حاول الخروج تم الدخول مرة أخرى.</strong>",
'importing'                 => 'جاري استيراد $1',
'editing'                   => 'تحرير $1',
'editinguser'               => 'تحرير $1',
'editingsection'            => 'تحرير $1 (قسم)',
'editingcomment'            => 'تحرير $1 (تعليق)',
'editconflict'              => 'تضارب في التحرير: $1',
'explainconflict'           => 'لقد قام أحد ما بتعديل الصفحة بعد أن بدأت انت بتحريرها.
صندوق النصوص العلوي يحتوي على النص الموجود حاليا في الصفحة.
والتغييرات التي قمت أنت بها موجودة في الصندوق في أسفل الصفحة.
يجب أن تقوم بدمج تغييراتك في النص الموجود حاليا.
<b>فقط</b> ما هو موجود في الصندوق العلوي هو ما سيتم حفظه وإستعاله عند الضغط على زر "حفظ الصفحة".
<p>',
'yourtext'                  => 'النص الذي كتبته',
'storedversion'             => 'النسخة المخزنة',
'nonunicodebrowser'         => '<strong>تحذير: متصفحك لا يوافق الترميز الموحد. تمت معالجة هذا لكي تتمكن من تحرير الصفحات بأمان: حروف ليست من ASCII سوف تظهر في صندوق التحريرx كأكواد سداسي عشرية.</strong>',
'editingold'                => '<strong> تحذير: أنت تقوم الآن بتحرير نسخة قديمة من هذه الصفحة. إذا قمت بحفظها، سيتم فقدات كافة التغييرات التي حدثت بعد هذه النسخة. </strong>',
'yourdiff'                  => 'الفروقات',
'copyrightwarning'          => 'يرجى الملاحظة أن جميع المساهمات هنا خاضعة وصادرة تحت ترخيص $2 (انظر في $1 للمزيد من التفاصيل)
إذا لم ترد أن تخضع كتابتك للتعديل والتوزيع الحر، لا تضعها هنا<br />. كما أنك تتعهد بأنك قمت بكتابة ما هو موجود بنفسك، أو قمت بنسخها من مصدر يخضع ضمن الملكية العامة، أو مصدر حر آخر. <strong>لا ترسل أي عمل ذي حقوق محفوظة بدون الإذن من صاحب الحق</strong>.',
'copyrightwarning2'         => 'من فضلك لاحظ أن حميع المشاركات في {{SITENAME}} يمكن أن تعدل أو تتغير أو تحذف من قبل المشاركين الآخرين. إذا لم تكن ترغب أن تعدل مشاركاتك بهذا الشكل, لا تضعها هنا.<br />
انت أيضا تقر أنك كتبت هذا بنفسك, أو نسختها من مصدر لا يخضع لحقوق النشر أو يسمح بالنقل بشكلا ما (أنظر $1 للتفاصيل).
<strong>لا تضف أي نص ذي حقوق محفوظة!</strong>',
'longpagewarning'           => '<strong>تنبيه: هذه صفحة طويلة وحجمها $1 كيلوبايت، بعض متصفحات الشبكة قد تواجه مشاكل عند محاولة تحرير صفحات يزيد حجمها عن 32 كيلوبايت. يرجى تقسيم الصفحة إلى أقسام أصغر حجمًا  إن أمكن.</strong>',
'longpageerror'             => '<strong>خطأ: النص الذي أدخلته يجاوز $1 كيلوبايت 
، و هذا أكبر من الحد الأقصى و هو $2 كيلوبايت. لا يمكن حفظ الصفحة.</strong>',
'readonlywarning'           => '<strong>تحذير: لقد أغلقت قاعدة البيانات للصيانة، لذلك لن تتمكن من حفظ التعديلات التي قمت بها حاليا. إذا رغبت بإمكانك أن تنسخ النص الذي تعمل عليه وتحفظه في ملف نصي إلى وقت لاحق.</strong>',
'protectedpagewarning'      => '<strong>تحذير: هذه الصفحة محمية. فقط المستخدمون ذوو الصلاحيات الإدارية يمكن أن يقوموا بتحريرها. تأكد من اتباع [[Project:تعليمات الصفحات المحمية|التعليمات الخاصة بالصفحات المحمية]] قبل التعديل.</strong>',
'semiprotectedpagewarning'  => "'''ملاحظة:''' هذه الصفحة محمية بحيث يتم تعديلها فقط من قبل المستخدمين المسجلين بالموسوعة.",
'cascadeprotectedwarning'   => '<strong>تحذير: تمت حماية هذه الصفحة بحيث يستطيع المستخدمون ذوو الصلاحيات الإدارية فقط تعديلها، وذلك لكونها مدمجة في الصفحات التالية والتي تمت حمايتها بخاصية "حماية الصفحات المدمجة":</strong>',
'templatesused'             => 'القوالب المستخدمة في هذه الصفحة:',
'templatesusedpreview'      => 'القوالب المستخدمة في هذا العرض المسبق:',
'templatesusedsection'      => 'القوالب المستخدمة في هذا القسم:',
'template-protected'        => '(حماية كاملة)',
'template-semiprotected'    => '(حماية جزئية)',
'edittools'                 => '<!-- النص هنا سيظهر تحت صناديق التحرير و رفع الصور. -->',
'nocreatetitle'             => 'تم تحديد إنشاء الصفحات',
'nocreatetext'              => 'قام الموقع بتحديد القدرة على إنشاء صفحات جديدة.
يمكنك العودة و تحرير صفحة موجودة بالفعل, أو [[Special:Userlogin|الدخول أو تسجيل حساب]].',

# "Undo" feature
'undo-success' => 'تم استرجاع التعديل. من فضلك، أكد التعديل وقم بحفظ الصفحة أدناه.',
'undo-failure' => 'لم ينجح الاسترجاع بسبب تعديلات متعارضة تمت على الصفحة.',
'undo-summary' => 'الرجوع عن التعديل $1 بواسطة [[Special:Contributions/$2|$2]] ([[نقاش المستخدم:$2|نقاش]])',

# Account creation failure
'cantcreateaccounttitle' => 'لم يتم إنشاء الحساب.',
'cantcreateaccounttext'  => 'تم منع إنشاء حساب خاص من عنوانك هذا (<b>$1</b>).
ربما يكون هذا بسبب التخريب الصادر من مدرستك أو مزود الإنترنت الخاص بك.',

# History pages
'revhistory'          => 'تاريخ التغييرات',
'viewpagelogs'        => 'معاينة السجلات لهذه الصفحة',
'nohistory'           => 'لا يوجد تاريخ للتغييرات لهذه الصفحة.',
'revnotfound'         => 'النسخة غير موجودة',
'revnotfoundtext'     => 'لم يتم العثور على النسخة القديمة من الصفحة التي طلبتها.
من فضلك تأكد من العنوان الذي دخلت به إلى هذه الصفحة.',
'loadhist'            => 'تحميل تاريخ الصفحة',
'currentrev'          => 'النسخة الحالية',
'revisionasof'        => 'نسخة $1',
'revision-info'       => 'تعديل $1 بواسطة $2',
'previousrevision'    => '&rarr; نسخة أقدم',
'nextrevision'        => 'نسخة أحدث &larr;',
'currentrevisionlink' => 'رؤية النسخة الحالية',
'cur'                 => 'الحالي',
'next'                => 'التالي',
'last'                => 'السابق',
'orig'                => 'الأصلي',
'page_first'          => 'الأولى',
'page_last'           => 'الأخيرة',
'histlegend'          => 'مفتاح: (الحالي) = الفرق مع النسخة الحالية
(السابق) = الفروقات مع النسخة السابقة، ط = تغيير طفيف',
'deletedrev'          => '[محذوف]',
'histfirst'           => 'أول',
'histlast'            => 'آخر',
'historysize'         => '($1 بايت)',
'historyempty'        => '(فارغ)',

# Revision feed
'history-feed-title'          => 'تاريخ التعديل',
'history-feed-description'    => 'تاريخ التعديل على هذه الصفحة في الويكي',
'history-feed-item-nocomment' => '$1 في $2', # user at time
'history-feed-empty'          => 'الصفحة المطلوبة غير موجودة. من المحتمل أن تكون هذه الصفحة قد حذفت أو نقلت. حاول [[Special:Search|البحث في الويكي]] عن صفحات جديدة ذات صلة.',

# Revision deletion
'rev-deleted-comment'         => '(تم حذف تعليق)',
'rev-deleted-user'            => '(تم حذف اسم مستخدم)',
'rev-deleted-event'           => '(تم مسح المدخلة)',
'rev-deleted-text-permission' => '<div class="mw-warning plainlinks">
تم مسح هذا التعديل من الأرشيف العام.
من الممكن أن يكون هناك تفاصيل حول هذا في [{{fullurl:Special:Log/delete|page={{FULLPAGENAMEE}}}} سجل الحذف].
</div>',
'rev-deleted-text-view'       => '<div class="mw-warning plainlinks">
تم مسح هذا التعديل من الأرشيف العام.
بإمكانك رؤية هذا التعديل لكونك إداريا على هذا الموقع.
من الممكن أن يكون هناك تفاصيل حول هذا في [{{fullurl:Special:Log/delete|page={{FULLPAGENAMEE}}}} سجل الحذف].
</div>',
'rev-delundel'                => 'عرض/إخفاء',
'revisiondelete'              => 'امسح/استرجع التعديلات',
'revdelete-nooldid-title'     => 'لا يوجد تعديل مختار',
'revdelete-nooldid-text'      => 'لم تقم بتحديد تعديل أو تعديلات معينة لعمل هذا الاجراء عليها.',
'revdelete-selected'          => "{{PLURAL:$2|النسخة المختارة|النسختان المختارتان|النسخ المختارة}} من '''$1:'''",
'logdelete-selected'          => "{{PLURAL:$2|الحدث المختار|الحدثان المختاران|الأحداث المختارة}} من السجل لـ '''$1:'''",
'revdelete-text'              => 'ستظل الأحداث والتعديلات الملغاة تظهر في تاريخ الصفحة والسجلات،
ولكن لن يكون مسموحا للعامة الدخول إلى بعض محتوياتها.

سيظل لدى الإداريين الآخريين بهذه الويكي الصلاحية لرؤية المحتويات المخفاة
ويستطيعون أيضا استرجاعها مرة أخرى من خلال نفس هذه الواجهة، ما لم تكن هناك ضوابط إضافية تم وضعها.',
'revdelete-legend'            => 'وضع ضوابط:',
'revdelete-hide-text'         => 'إخفاء نص التعديل',
'revdelete-hide-name'         => 'إخفاء الإجراء والهدف منه',
'revdelete-hide-comment'      => 'إخفاء تعليق التعديل',
'revdelete-hide-user'         => 'إخفاء اسم/عنوان المستخدم',
'revdelete-hide-restricted'   => 'تطبيق هذه الضوابط على الإداريين بجانب الآخرين',
'revdelete-suppress'          => 'إخفاء البيانات من الإداريين بجانب الآخرين',
'revdelete-hide-image'        => 'إخفاء محتويات الملف',
'revdelete-unsuppress'        => 'إزالة الضوابط من التعديلات المسترجعة',
'revdelete-log'               => 'تعليق السجل:',
'revdelete-submit'            => 'تطبيق على التعديل المختار',
'revdelete-logentry'          => 'تغيير مرئية تعديلات في [[$1]]',
'logdelete-logentry'          => 'تغيير مرئية أحداث في [[$1]]',
'revdelete-logaction'         => 'تم ضبط {{plural:$1|تعديل|تعديلان|$1 تعديلات|$1 تعديل|$1 تعديلا}} للوضع $2',
'logdelete-logaction'         => 'تم ضبط {{plural:$1|حدث|حدثين|$1 أحداث|$1 حدث|$1 حدثا}} بخصوص [[$3]] للوضع $2',
'revdelete-success'           => 'تم ضبط مرئية التعديلات بنجاح.',
'logdelete-success'           => 'تم ضبط مرئية الأحداث بنجاح.',

# Oversight log
'overlogpagetext' => 'القائمة الموجودة هنا هي أحدث عمليات الحذف والمنع المتضمنة محتويات تم إخفائها عن الإداريين. أنظر [[Special:Ipblocklist|قائمة منع عناوين الأيبي]] لترى حالات المنع والطرد الحالية.',

# Diffs
'difference'                => '(الفرق بين النسخ)',
'loadingrev'                => 'تحميل التعديلات للمقارنة',
'lineno'                    => 'سطر $1:',
'editcurrent'               => 'حرر النسخة الحالية من هذه الصفحة',
'selectnewerversionfordiff' => 'اختر نسخة أحدث للمقارنة',
'selectolderversionfordiff' => 'اختر نسخة أقدم للمقارنة',
'compareselectedversions'   => 'قارن بين النسخ المختارة',
'editundo'                  => 'استرجاع',
'diff-multi'                => '(عدد التعديلات غير الظاهرة هو $1.)',

# Search results
'searchresults'         => 'نتائج البحث',
'searchresulttext'      => 'للمزيد من المعلومات حول البحث في {{SITENAME}}، راجع [[{{MediaWiki:helppage}}|تصفح]].',
'searchsubtitle'        => 'لصيغة البحث "[[$1]]"',
'searchsubtitleinvalid' => 'لصيغة البحث "$1"',
'badquery'              => 'نص بحث خاطئ',
'badquerytext'          => 'لا يمكننا معالجة طلبك. من المحتمل أنك حاولت البحث عن كلمة بأقل من ثلاثة حروف, و هذا غير مدعم حاليا. كما قد يكون ناتجا عن خلط في أسلوب البحث, مثلا: "سمك و و تفاح". المرجو تجريب شيء آخر.',
'matchtotals'           => 'الاستعلام "$1" يشابه $2 عنوان صفحة
و نصوص $3 صفحة.',
'noexactmatch'          => 'لا يوجد صفحة بنفس العنوان، حاول البحث بشكل مفصل أكثر من خلال استعمال صندوق البحث أدناه. بإمكانك أيضاً إنشاء [[$1|صفحة جديدة]] بالعنوان الذي طلبته.',
'titlematches'          => 'نتائج بحث العناوين',
'notitlematches'        => 'لم يتم إيجاد أي عنوان مطابق',
'textmatches'           => 'نتائج بحث نصوص الصفحات',
'notextmatches'         => 'لم يتم إيجاد أي نص مطابق',
'prevn'                 => '$1 السابقة',
'nextn'                 => '$1 التالية',
'viewprevnext'          => 'عرض ($1) ($2) ($3).',
'showingresults'        => 'القائمة التالية تظهر <b>$1</b> نتيجة أو أقل بدءًا من المدخلة رقم <b>$2</b>.',
'showingresultsnum'     => 'عرض <b>$3</b> نتيجة بدءًا من المدخلة رقم <b>$2</b>.',
'nonefound'             => "'''ملاحظة:''' عمليات البحث غير الناجحة تكون عادة نتيجة للبحث عن كلمات شائعة مثل \"from\" و \"have\" والتي لا يتمّ فهرستها، وقد تنتج أيضًا من استعمال أكثر من كلمة في البحث (فقط الصفحات التي تحتوي على جميع هذه الكلمات سوف تظهر في نتيجة البحث).",
'powersearch'           => 'بحث',
'powersearchtext'       => 'ابحث في النطاقات:<br />$1<br />$2 عرض التحويلات<br />ابحث عن $3 $9',
'searchdisabled'        => '<p>عذرا! لقد تم إيقاف ميزة البحث في النصوص بشكل مؤقت، لأسباب تتعلق بتأثيرها على الأداء العام. في الوقت الحالي من الممكن أن تستعمل محرك البحث جووجل Google بدل من خاصية البحث في النصوص. من الممكن أن لا يكون البحث في جووجل يشمل آخر التعديلات والصفحات.
</p>',
'blanknamespace'        => 'مقالات',

# Preferences page
'preferences'              => 'تفضيلات',
'mypreferences'            => 'تفضيلاتي',
'prefsnologin'             => 'غير مسجل',
'prefsnologintext'         => 'يجب أن تكون [[Special:Userlogin|مسجلا]] حتى تتمكن من تعديل تفضيلات المستخدم.',
'prefsreset'               => 'أعيدت التفضيلات إلى الإعداد الافتراضي المخزن.',
'qbsettings'               => 'خيارات لوحة الوصلات',
'qbsettings-none'          => 'بلا تحديد',
'qbsettings-fixedleft'     => 'مثبت لليسار',
'qbsettings-fixedright'    => 'مثبت لليمين',
'qbsettings-floatingleft'  => 'حر لليسار',
'qbsettings-floatingright' => 'حر لليمين',
'changepassword'           => 'غير كلمة السر',
'skin'                     => 'واجهة',
'math'                     => 'رياضيات',
'dateformat'               => 'صيغة التاريخ',
'datedefault'              => 'لا تفضيل',
'datetime'                 => 'وقت وتاريخ',
'math_failure'             => 'خطأ رياضيات',
'math_unknown_error'       => 'خطأ مجهول',
'math_unknown_function'    => 'وظيفة مجهولة',
'math_lexing_error'        => 'خطأ في الصيغة',
'math_syntax_error'        => 'خطأ بالصيغة',
'math_image_error'         => 'فشل التحويل لPNG , تحقق من تثبيت كل من Latex و dvips و gs و convert.',
'math_bad_tmpdir'          => 'لا يمكن انشاء أو كتابة في الملف المؤقت',
'math_bad_output'          => 'لا يمكن انشاء أو كتابة في ملف الخروج',
'prefs-personal'           => 'معلومات المستخدم',
'prefs-rc'                 => 'أحدث التغييرات',
'prefs-watchlist'          => 'قائمة المراقبة',
'prefs-watchlist-days'     => 'عدد الأيام المعروضة في قائمة المراقبة:',
'prefs-watchlist-edits'    => 'عدد التعديلات التي تعرض في قائمة المراقبة الموسعة:',
'prefs-misc'               => 'متفرقات',
'saveprefs'                => 'حفظ التفضيلات',
'resetprefs'               => 'أعد التعديلات الأصلية',
'oldpassword'              => 'كلمة السر القديمة',
'newpassword'              => 'كلمة السر الجديدة',
'retypenew'                => 'أعد كتابة كلمة السر الجديدة',
'textboxsize'              => 'أبعاد صندوق النصوص',
'rows'                     => 'أسطر',
'columns'                  => 'أعمدة',
'searchresultshead'        => 'خيارات نتائج البحث',
'resultsperpage'           => 'عدد النتائج في الصفحة',
'contextlines'             => 'عدد الأسطر في كل نتيجة',
'contextchars'             => 'عدد الأحرف في كل سطر',
'stubthreshold'            => 'الحدّ الأدنى لعرض البذرة',
'recentchangescount'       => 'عدد العناوين في صفحة أحدث التغييرات',
'savedprefs'               => 'تم حفظ تفضيلاتك.',
'timezonelegend'           => 'المنطقة الزمنية',
'timezonetext'             => 'عدد الساعات بين توقيتك المحلي والتوقيت على الخادم (UTC).',
'localtime'                => 'عرض الوقت المحلي',
'timezoneoffset'           => 'الفرق¹',
'servertime'               => 'الوقت في الأجهزة الخادمة الآن هو',
'guesstimezone'            => 'ادخل التوقيت من المتصفح',
'allowemail'               => 'السماح بتلقي رسائل الكترونية من المستخدمين الآخرين',
'defaultns'                => 'أبحث في هذه النطاقات بشكل أفتراضي:',
'default'                  => 'مبدئي',
'files'                    => 'ملفات',

# User rights
'userrights-lookup-user'     => 'إدارة مجموعات المستخدمين',
'userrights-user-editname'   => 'أدخل اسم مستخدم:',
'editusergroup'              => 'عدل مجموعات المستخدمين',
'userrights-editusergroup'   => 'تعديل مجموعات المستخدمين',
'saveusergroups'             => 'احفظ مجموعات المستخدم',
'userrights-groupsmember'    => 'عضو في:',
'userrights-groupsavailable' => 'المجموعات المتاحة:',
'userrights-groupshelp'      => 'قم باختيار المجموعات التي تريد المستخدم أن يضاف إليها أو تتم إزالته منها.
المجموعات الغير مختارة ستترك كما هي. يمكنك إزالة مجموعة بالضغط على كنترل + ضغطة فأرة يسرى.',

# Groups
'group'            => 'المجموعة:',
'group-bot'        => 'بوت',
'group-sysop'      => 'إداريون',
'group-bureaucrat' => 'بيروقراطيون',
'group-all'        => '(الكل)',

'group-bot-member'        => 'بوت',
'group-sysop-member'      => 'إداري',
'group-bureaucrat-member' => 'بيروقراط',

'grouppage-bot'        => 'Project:بوت',
'grouppage-sysop'      => 'Project:إداريين',
'grouppage-bureaucrat' => 'Project:بيروقراط',

# User rights log
'rightslog'      => 'سجل صلاحيات المستخدمين',
'rightslogtext'  => 'هذه قائمة بالتغييرات في صلاحيات المستخدمين.',
'rightslogentry' => 'تغيير صلاحيات $1 من $2 إلى $3',
'rightsnone'     => 'مستخدم عادي',

# Recent changes
'nchanges'                          => '{{PLURAL:$1|تغيير|تغييران|$1 تغييرات|$1 تغيير|$1 تغييرا}}',
'recentchanges'                     => 'أحدث التغييرات',
'recentchangestext'                 => 'تابع آخر التغييرات في الموسوعة من هذه الصفحة.',
'recentchanges-feed-description'    => 'تابع أحدث التغييرات لهذه الويكي عبر هذه التلقيمة.',
'rcnote'                            => 'ستجد بالأسفل آخر <strong>$1</strong> تعديل في آخر <strong>$2</strong> يوم, منذ $3.',
'rcnotefrom'                        => 'في الأسفل التغييرات منذ <b>$2</b> (ولغاية <b>$1</b>).',
'rclistfrom'                        => 'أظهر التغييرات بدأ من $1',
'rcshowhideminor'                   => '$1 التعديلات الطفيفة',
'rcshowhidebots'                    => '$1 تعديلات البوت',
'rcshowhideliu'                     => '$1 تعديلات المستخدمين المسجلين',
'rcshowhideanons'                   => '$1 تعديلات المستخدمين المجهولين',
'rcshowhidepatr'                    => '$1 التعديلات المراجعة',
'rcshowhidemine'                    => '$1 تعديلاتي',
'rclinks'                           => 'أظهر آخر $1 تعديل في آخر $2 يوم، $3',
'diff'                              => 'فرق',
'hist'                              => 'تاريخ',
'hide'                              => 'إخفاء',
'show'                              => 'عرض',
'minoreditletter'                   => 'ط',
'newpageletter'                     => 'ج',
'boteditletter'                     => 'ب',
'sectionlink'                       => '←',
'number_of_watching_users_pageview' => '[$1 مستخدم مراقب]',
'rc_categories'                     => 'حصر لتصنيفات (مفرقة برمز "|")',
'rc_categories_any'                 => 'أي',

# Recent changes linked
'recentchangeslinked'          => 'تغييرات ذات علاقة',
'recentchangeslinked-noresult' => 'لم تحدث تعديلات في الصفحات التي لها وصلات هنا خلال الفترة المحددة.',

# Upload
'upload'                      => 'ارفع ملفا',
'uploadbtn'                   => 'رفع الملف',
'reupload'                    => 'إعادة الرفع',
'reuploaddesc'                => 'الرجوع لصفحة الرفع.',
'uploadnologin'               => 'لم تقم بتسجيل الدخول',
'uploadnologintext'           => 'يجب أن تكون [[Special:Userlogin|مسجلا الدخول]] لتتمكن من رفع الملفات.',
'upload_directory_read_only'  => 'لا يمكن الكتابة على مجلد الرفع ($1) من قبل خادم الشبكة.',
'uploaderror'                 => 'خطأ في الرفع',
'uploadtext'                  => "استعمل الصندوق بالأسفل لرفع الصور, لعرض أو البحث في الصور المحملة سابقا, راجع [[Special:Imagelist|قائمة الملفات المرفوعة]], سجل الرفع و المسح موجود في [[Special:Log/upload|سجل الرفع]].

لادراج صورة في صفحة, استعمل الوصلات في الصيغ التالية:
*'''<nowiki>[[{{ns:image}}:ملف.jpg]]</nowiki>''',
*'''<nowiki>[[{{ns:image}}:ملف.png|alt text]]</nowiki>''' أو
*'''<nowiki>[[{{ns:media}}:ملف.ogg]]</nowiki>''' للوصل للملف مباشرة.",
'uploadlog'                   => 'سجل الرفع',
'uploadlogpage'               => 'سجل الرفع',
'uploadlogpagetext'           => 'في الأسفل قائمة بآخر الملفات التي تم رفعها.
كل الأوقات المعروضة هي حسب توقيت الأجهزة الخادمة (UTC).',
'filename'                    => 'اسم الملف',
'filedesc'                    => 'وصف قصير',
'fileuploadsummary'           => 'نبذة:',
'filestatus'                  => 'حالة الترخيص',
'filesource'                  => 'مصدر',
'uploadedfiles'               => 'الملفات المحملة',
'ignorewarning'               => 'تغاض عن هذا التحذير واحفظ الملف.',
'ignorewarnings'              => 'أهمل أية تحذيرات',
'minlength'                   => 'اسم الملفات يجب أن لا يقل عن ثلاثة أحرف.',
'illegalfilename'             => 'يحتوي اسم الملف $1 على رموز غير مقبولة. أعد تسمية الملف من فضلك و قم برفعه مرة أخرى.',
'badfilename'                 => 'تم تغيير اسم الصورة إلى "$1".',
'filetype-badmime'            => 'من غير المسموح به رفع ملفات من النوع "$1".',
'filetype-badtype'            => "يعتبر '''\".\$1\"''' من أنواع الملفات المرفوضة
: قائمة بالأنواع المسموح بها: \$2",
'filetype-missing'            => 'الملف بدون إمتداد (مثل ".jpg").',
'large-file'                  => 'ينصح ألا تكون الملفات أكبر من $1؛ هذا الملف مساحته $2.',
'largefileserver'             => 'حجم هذا الملف أكبر من المسموح به على هذا الخادم (Server).',
'emptyfile'                   => 'يبدو أن هذا الملف فارغ. قد يكون سبب ذلك خطأ في كتابة الاسم. الرجاء التحقق من اسم الملف.',
'fileexists'                  => 'هنالك ملف موجود يحمل نفس هذا الاسم الرجاء التأكد من هذا الملف باتّباع الوصلة التالية ($1) قبل القيام بتغييره.',
'fileexists-extension'        => 'يوجد ملف باسم مشابه:<br />
اسم الملف المراد رفعه: <strong><tt>$1</tt></strong><br />
اسم الملف الموجود بالفعل: <strong><tt>$2</tt></strong><br />
الاختلاف الوحيد بينهما هو حالة أحرف الإمتداد من حيث الكبر أو الصغر. افحص الملفين من أجل التحقق من عدم تطابقهما من فضلك.',
'fileexists-thumb'            => "'''<center>الصورة الموجودة</center>'''",
'fileexists-thumbnail-yes'    => 'يبدو أن الملف مصغرا لحجم أعلى <i>(تصغير)</i>. افحص الملف <strong><tt>$1</tt></strong> من فضلك.<br />
إن كان هو نفس الملف بالحجم الأصلي فلا داعي لرفع تصغير إضافي.',
'file-thumbnail-no'           => 'يبدأ الملف بـ <strong><tt>$1</tt></strong>. يبدو أن الملف مصغرا لحجم أعلى <i>(تصغير)</i>.
إذا كانت لديك الصورة في درجة استبانة كاملة قم برفعها، أو قم بتغيير اسم الملف من فضلك.',
'fileexists-forbidden'        => 'هنالك ملف بنفس الاسم حاليا؛ من فضلك تراجع و اعطه اسما جديداً. [[صورة:$1|تصغير|وسط|$1]]',
'fileexists-shared-forbidden' => 'يوجد ملف بنفس الاسم في مستودع الملفات المشترك (كومنز)، نرجو أن تعود و تحمل الملف تحت اسم جديد إذا أردت. [[صورة:$1|تصغير|وسط|$1]]',
'successfulupload'            => 'تم رفع الملف بنجاح',
'fileuploaded'                => "تمّ رفع الملف $1 بنجاح. الرجاء اتّباع الوصلة التالية: $2 للذهاب إلى صفحة وصف الملف وإضافة معلومات عنه. يمكنك إضافة معلومات عن مصدر الملف وتاريخ إنشائها وأي معلومات أخرى عنها. إذا كان هذا الملف عبارة عن صورة فبإمكانك استعماله في المقالات بإضافة النص التالي:

'''<tt><nowiki>[[</nowiki>{{ns:image}}<nowiki>:$1|تصغير|عنوان الصورة]]</nowiki></tt>'''",
'uploadwarning'               => 'تحذير رفع الملفات',
'savefile'                    => 'حفظ الملف',
'uploadedimage'               => 'تم رفع "[[$1]]"',
'uploaddisabled'              => 'عذرا، تم إيقاف خاصية رفع الملفات.',
'uploaddisabledtext'          => 'تم منع رفع الملفات على هذه الويكي.',
'uploadscripted'              => 'يضم هذا الملف قن HTML أو قن آخر يمكنه أن يأول بطريقة خاطئة بواسطة المتصفح.',
'uploadcorrupt'               => 'هذا الملف غير سليم أو له امتداد خاطيء. الرجاء التأكد من الملف وإعادة الرفع مرة أخرى.',
'uploadvirus'                 => 'هذا الملف يحتوي على فيروس. التفاصيل: $1',
'sourcefilename'              => 'الملف',
'destfilename'                => 'تخرين الملف باسم',
'watchthisupload'             => 'راقب هذه الصفحة',
'filewasdeleted'              => 'تم رفع ثم مسح ملف بهذا الاسم من قبل. من الأفضل مراجعة $1 قبل رفعه مرة أخرى.',

'upload-proto-error'      => 'بروتوكول غير صحيح',
'upload-proto-error-text' => 'الرفع عن بعد يتطلب مسارا يبدأ بـ <code>http://</code> أو <code>ftp://</code>.',
'upload-file-error'       => 'خطأ داخلي',
'upload-file-error-text'  => 'تم وقوع خطأ داخلي عند محاولة عمل ملف مؤقت على الخادم. قم بمراسلة مدير النظام من فضلك.',
'upload-misc-error'       => 'خطأ في الرفع غير معلوم',
'upload-misc-error-text'  => 'حدث خطأ غير معلوم أثناء عملية الرفع. من فضلك تاكد أن المسار صحيح ومن الممكن الدخول عليه ثم حاول مرة أخرى. إذا استمرت المشكلة في الحدوث، راسل مديرا للنظام.',

# Some likely curl errors. More could be added from <http://curl.haxx.se/libcurl/c/libcurl-errors.html>
'upload-curl-error6'       => 'لم يتمكن من الوصول للمسار',
'upload-curl-error6-text'  => 'لم يتمكن من الوصول إلى المسار الذي حددته.  راجع من فضلك صحة المسار مرة أخرى وأن الموقع يعمل.',
'upload-curl-error28'      => 'انتهاء مهلة الرفع',
'upload-curl-error28-text' => 'أخذ الموقع وقت طويل للاستجابة. تأكد أن الموقع يعمل وانتظر فترة قصيرة ثم حاول مرة أخرى. ربما يتعين عليك المحاولة في وقت أقل ازدحاما.',

'license'            => 'ترخيص',
'nolicense'          => 'غير محدّد',
'upload_source_url'  => '(عنوان ويب صحيح، يمكن الوصول إليه)',
'upload_source_file' => ' (ملف على حاسبك)',

# Image list
'imagelist'                 => 'قائمة الصور',
'imagelisttext'             => 'فيما يلي قائمة تحوي $1 ملفًا مرتبة $2.',
'imagelistforuser'          => 'يعرض هذا الصور التي قام $1 برفعها.',
'getimagelist'              => 'جاري إحضار قائمة الملفات',
'ilsubmit'                  => 'بحث',
'showlast'                  => 'عرض آخر $1 ملف بترتيب $2.',
'byname'                    => 'حسب الاسم',
'bydate'                    => 'على التاريخ',
'bysize'                    => 'على الحجم',
'imgdelete'                 => 'حذف',
'imgdesc'                   => 'وصف',
'imgfile'                   => 'ملف',
'imglegend'                 => 'مفتاح: (وصف) = عرض أو تعديل صفحة الوصف.',
'imghistory'                => 'تاريخ الصورة',
'revertimg'                 => 'استرجاع',
'deleteimg'                 => 'احذف',
'deleteimgcompletely'       => 'احذف كل نسخ هذا الملف',
'imghistlegend'             => 'مفتاح: (حالي) = النسخة الموجودة حاليًا من الملف، (حذف) = حذف هذه النسخة السابقة، (استرجاع) = استرجاع نسخة قديمة من الملف.
<br /><i>اضغط على التاريخ لرؤية النسخة المحملة في ذلك التاريخ.</i>.',
'imagelinks'                => 'وصلات',
'linkstoimage'              => 'الصفحات التالية تحتوي على وصلة لهذه الصورة:',
'nolinkstoimage'            => 'لا يوجد صفحات تصل لهذه الصورة.',
'sharedupload'              => 'هذا الملف تم رفعه للتشارك بين المشاريع ويمكن استعماله في المشاريع الأخرى',
'shareduploadwiki'          => 'الرجاء رؤية $1 لمزيد من المعلومات.',
'shareduploadwiki-linktext' => 'صفحة وصف الملف',
'noimage'                   => 'لا يوجد ملف بهذا الاسم، بإمكانك أن تقوم بـ$1.',
'noimage-linktext'          => 'رفعه',
'uploadnewversion-linktext' => 'رفع نسخة جديدة للملف',
'imagelist_date'            => 'تاريخ',
'imagelist_name'            => 'اسم',
'imagelist_user'            => 'مستخدم',
'imagelist_size'            => 'حجم (بايت)',
'imagelist_description'     => 'وصف',
'imagelist_search_for'      => 'ابحث عن اسم الصورة:',

# MIME search
'mimesearch'         => 'بحث MIME',
'mimesearch-summary' => 'تعمل هذه الصفحة على عرض وتصفية الملفات بناءا على نوعها. المدخل: نوع المحتوى/النوع الفرعي، كمثال
<tt>image/jpeg</tt>.',
'mimetype'           => 'نوع الملف:',
'download'           => 'أنزل',

# Unwatched pages
'unwatchedpages' => 'صفحات غير مراقبة',

# List redirects
'listredirects' => 'عرض التحويلات',

# Unused templates
'unusedtemplates'     => 'قوالب غير مستعملة',
'unusedtemplatestext' => 'تحتوي هذه الصفحة على قائمة بالصفحات من نطاق «قالب» غير المستعملة في صفحات أخرى. تذكر بأن تتحقق من عدم وجود وصلات أخرى لهذه القوالب قبل حذفها.',
'unusedtemplateswlh'  => 'وصلات أخرى',

# Random redirect
'randomredirect' => 'صفحة تحويل عشوائية',

# Statistics
'statistics'             => 'إحصاءات',
'sitestats'              => 'إحصاءات الموقع',
'userstats'              => 'احصائيات المستخدم',
'sitestatstext'          => "يوجد '''\$1''' صفحة كعدد كلي في قاعدة البيانات.
و هذا يتضمن \"صفحات النقاش\", الصفحات حول {{SITENAME}}, صفحات \"البذور\" الصغيرة
, صفحات التحويل, و صفحات أخرى ربما لا ترقى لمستوى مقالة.
بخلاف هذه الصفحات, يوجد '''\$2''' صفحة يمكن أن يعدوا كصفحات تحتوي على مقالات.

تم تحميل '''\$8''' ملف.

تم عرض الصفحات '''\$3''' مرة , و تم عمل '''\$4''' تعديل للصفحات
منذ بداية عمل الويكي.
و هذا بمعدل '''\$5''' للصفحة الواحدة, و '''\$6''' مشاهدة للتعديل الواحد.

طول [http://meta.wikimedia.org/wiki/Help:Job_queue المهمات] هو '''\$7'''.",
'userstatstext'          => "هناك {{PLURAL:$1|مستخدم مسجل وحيد|مستخدمين مسجلين|$3 مستخدمين مسجلين|$3 مستخدم مسجل|$3 مستخدما مسجلا}}, منهم
'''$2''' (أو '''$4%''') {{PLURAL:$2|لديه|لديهما|لديهم}} صلاحيات $5.",
'statistics-mostpopular' => 'أكثر الصفحات مشاهدة',

'disambiguations'      => 'صفحات التوضيح',
'disambiguationspage'  => 'قالب:توضيح',
'disambiguations-text' => "الصفحات التالية تصل إلى '''صفحة توضيح'''. ينبغي في المقابل أن تصل إلى الضفحة الملائمة. <br />تعامل الصفحة كصفحة توضيح إذا كان بها قالب موجود في [[MediaWiki:disambiguationspage|صفحة الميدياويكي هذه]]",

'doubleredirects'     => 'تحويلات مزدوجة',
'doubleredirectstext' => "كل سطر في القائمة التالية يحتوي على وصلات للتحويلة الأولى والثانية بالإضافة إلى أول سطر من نص التحويلة الثانية والذي عادة ما يشير إلى المقالة ''المفترض'' أن يتم التحويل إليها مباشرة من التحويلة الأولى أيضا.",

'brokenredirects'        => 'تحويلات مكسورة',
'brokenredirectstext'    => 'الوصلات التالية تشير لصفحات غير موجودة.',
'brokenredirects-edit'   => '(تحرير)',
'brokenredirects-delete' => '(مسح)',

'withoutinterwiki'        => 'صفحات بدون وصلات لغات أخرى',
'withoutinterwiki-header' => 'الصفحات التالية لا تصل إلى نسخ بلغات أخرى:',

'fewestrevisions' => 'المقالات ذات أقل تعديل',

# Miscellaneous special pages
'nbytes'                  => '{{PLURAL:$1|بايت واحد|$1 بايت|$1 بايت|$1 بايت}}',
'ncategories'             => '{{PLURAL:$1|تصنيف واحد|تصنيفان|$1 تصنيفات|$1 تصنيف|$1 تصنيفا}}',
'nlinks'                  => '{{PLURAL:$1|وصلة واحدة|وصلتان|$1 وصلات|$1 وصلة}}',
'nmembers'                => '{{PLURAL:$1|عضو واحد|عضوان|$1 أعضاء|$1 عضو|$1 عضوا}}',
'nrevisions'              => '{{PLURAL:$1|تعديل وحيد|تعديلان|$1 تعديلات|$1 تعديل|$1 تعديلا}}',
'nviews'                  => '{{PLURAL:$1|مشاهدة واحدة|مشاهدتان|$1 مشاهدات|$1 مشاهدة}}',
'specialpage-empty'       => 'هذه الصفحة فارغة.',
'lonelypages'             => 'صفحات يتيمة',
'lonelypagestext'         => 'الصفحات التالية لا تصل إليها أيا من صفحات الويكي الأخرى.',
'uncategorizedpages'      => 'صفحات غير مصنفة',
'uncategorizedcategories' => 'تصنيفات غير مصنفة',
'uncategorizedimages'     => 'صور غير مصنفة',
'unusedcategories'        => 'تصنيفات غير مستعملة',
'unusedimages'            => 'صور غير مستعملة',
'popularpages'            => 'الصفحات المشهورة',
'wantedcategories'        => 'تصنيفات مطلوبة',
'wantedpages'             => 'صفحات مطلوبة',
'mostlinked'              => 'أكثر الصفحات المرتبطة بمقالات أخرى',
'mostlinkedcategories'    => 'أكثر التصنيفات ارتباطا',
'mostcategories'          => 'أكثر المقالات تصنيفا',
'mostimages'              => 'أكثر الصور ارتباطا',
'mostrevisions'           => 'أكثر الصفحات تعديلا',
'allpages'                => 'كل الصفحات',
'prefixindex'             => 'فهرس',
'randompage'              => 'صفحة عشوائية',
'shortpages'              => 'صفحات قصيرة',
'longpages'               => 'صفحات طويلة',
'deadendpages'            => 'صفحات نهاية مسدودة',
'deadendpagestext'        => 'الصفحات التالية لا تحتوي على وصلات إلى صفحات أخرى بهذه الويكي.',
'protectedpages'          => 'صفحات محمية',
'protectedpagestext'      => 'الصفحات التالية محمية من التعديل أو النقل',
'protectedpagesempty'     => 'لا توجد صفحات محمية حاليا.',
'listusers'               => 'قائمة الأعضاء',
'specialpages'            => 'الصفحات الخاصّة',
'spheading'               => 'الصفحات الخاصة لكل المستخدمين',
'restrictedpheading'      => 'صفحات خاصة للمخولين فقط',
'rclsub'                  => '(لصفحات تصل بها الصفحة "$1")',
'newpages'                => 'صفحات جديدة',
'newpages-username'       => 'اسم المستخدم:',
'ancientpages'            => 'المقالات القديمة',
'intl'                    => 'وصلات بين لغات الموسوعة',
'move'                    => 'انقل',
'movethispage'            => 'أنقل هذه الصفحة',
'unusedimagestext'        => '<p>الرجاء الانتباه إلى أن بعض الصور الموجودة في هذه القائمة قد يكون مشار إليها باستخدام العنوان الكامل (URL) وهي قيد الاستعمال بالرغم من ادراجها هنا.</p>',
'unusedcategoriestext'    => 'التصنيفات التالية موجودة على الرغم من أنها لا تحتوي على أية مقالات أو تصنيفات أخرى.',

# Book sources
'booksources'               => 'مصدر كتاب',
'booksources-search-legend' => 'البحث عن مصادر الكتب',
'booksources-isbn'          => 'ردمك:',
'booksources-go'            => 'إذهب',
'booksources-text'          => 'توجد أدناه قائمة بوصلات لمواقع أخرى تبيع الكتب الجديدة والمستعملة، أيضا يمكنك أن تحصل على معلومات إضافية عن الكتب التي تبحث عنها من هناك:',

'categoriespagetext' => 'التصنيفات التالية موجودة في {{SITENAME}}',
'data'               => 'بيانات',
'userrights'         => 'إدارة صلاحيات المستخدمين',
'groups'             => 'مجموعات المستخدمين',
'isbn'               => 'ردمك',
'alphaindexline'     => '$1 إلى $2',
'version'            => 'رقم النسخة',

# Special:Log
'specialloguserlabel'  => 'المستخدم:',
'speciallogtitlelabel' => 'العنوان:',
'log'                  => 'سجل كامل',
'log-search-legend'    => 'ابحث عن سجلات',
'log-search-submit'    => 'اذهب',
'alllogstext'          => 'عرض شامل لعمليات رفع الملفات، وعمليات حذف أو حماية المقالات، وعمليات منع المستخدمين، وعمليات التغيير في الإداريين. باستطاعتك جعل القائمة أكثر تحديدًا وذلك باختيار نوع العملية، أو اسم المستخدم، أو الصفحات المتأثّرة.',
'logempty'             => 'لا يوجد مدخلات مطابقة في السجل.',
'log-title-wildcard'   => 'البحث عن عناوين تبدأ بهذا النص',

# Special:Allpages
'nextpage'          => 'الصفحة التالية ($1)',
'prevpage'          => 'الصفحة السابقة ($1)',
'allpagesfrom'      => 'عرض الصفحات بدءا من:',
'allarticles'       => 'جميع المقالات',
'allinnamespace'    => 'جميع الصفحات (في نطاق $1)',
'allnotinnamespace' => 'كل الصفحات (ليست في نطاق $1)',
'allpagesprev'      => 'السابق',
'allpagesnext'      => 'التالي',
'allpagessubmit'    => 'اذهب',
'allpagesprefix'    => 'عرض الصفحات التي تبدأ بـ:',
'allpagesbadtitle'  => 'العنوان المقترح للصفحة غير مقبول أو يضم لغات أخرى أو سابقة أنترويكي. يمكن ان يتضمن حروفا لا يمكن استعمالها للعناوين.',

# Special:Listusers
'listusersfrom'      => 'عرض المستخدمين بداية من:',
'listusers-submit'   => 'عرض',
'listusers-noresult' => 'لم يتم ايجاد مستخدم.',

# E-mail user
'mailnologin'     => 'لا يوجد عنوان للإرسال',
'mailnologintext' => 'يجب أن تقوم [[Special:Userlogin|بتسجيل الدخول]] وإدخال بريد إلكتروني صالح في صفحة [[Special:Preferences|التفضيلات]] لتتمكن من إرسال الرسائل لمستخدمين آخرين.',
'emailuser'       => 'أرسل رسالة لهذا المستخدم',
'emailpage'       => 'أرسل رسالة للمستخدم',
'emailpagetext'   => 'لو أن هذا المستخدم قد قام بإدخال عنوان بريدي صحيح في تفضيلاته،
فسيتم إرسال رسالة واحدة له بالنموذج أدناه.
العنوان الذي قمت أنت بإدخاله لك في تفضيلات المستخدم،
سيظهر في مكان المرسل في الرسالة التي سترسل له، ليتمكن من الرد عليك.',
'defemailsubject' => 'رسالة من {{SITENAME}}',
'noemailtitle'    => 'لا يوجد عنوان بريد إلكتروني',
'noemailtext'     => 'لم يحدد هذا المستخدم عنوان بريد إلكتروني صحيح،
أو طلب عدم استلام الرسائل من المستخدمين الآخرين.',
'emailfrom'       => 'من',
'emailto'         => 'إلى',
'emailsubject'    => 'العنوان',
'emailmessage'    => 'نص الرسالة',
'emailsend'       => 'إرسال',
'emailccme'       => 'أرسل لي بنسخة من رسالتي.',
'emailccsubject'  => 'نسخة من رسالتك إلى $1: $2',
'emailsent'       => 'تم إرسال الرسالة',
'emailsenttext'   => 'تم إرسال رسالتك الإلكترونية.',

# Watchlist
'watchlist'            => 'قائمة مراقبتي',
'mywatchlist'         => 'قائمة مراقبتي',
'watchlistfor'         => "(ل '''$1''')",
'nowatchlist'          => 'لا يوجد شيء في قائمة مراقبتك.',
'watchlistanontext'    => 'الرجاء $1 لعرض أو تعديل الصفحات في قائمة مراقبتك.',
'watchlistcount'       => "'''لديك $1 صفحة في قائمة مراقبتك ، متضمنة صفحات النقاش.'''",
'clearwatchlist'       => 'امسح ما في قائمة المراقبة',
'watchlistcleartext'   => 'هل ترغب فعلا بإزالتهم؟',
'watchlistclearbutton' => 'أفرغ قائمة المراقبة',
'watchlistcleardone'   => 'تم إفراغ قائمة مراقبتك. تم إزالة $1 صفحة.',
'watchnologin'         => 'غير مسجل',
'watchnologintext'     => 'يجب أن تقوم  [[Special:Userlogin|بتسجيل الدخول]] لتتمكن من تعديل قائمة المراقبة لديك.',
'addedwatch'           => 'تمت الإضافة لقائمة المراقبة',
'addedwatchtext'       => 'تمت إضافة الصفحة  "$1" إلى [[Special:Watchlist|قائمة المراقبة]] خاصتك. سيتم وضع التغييرات القادمة على هذه الصفحة، وصفحة النقاش الخاصة بها سيتم وضعها هناك. وسيتم اظهار اسم الصفحة بخط <b>عريض</b> في صفحة [[Special:Recentchanges|أحدث التغييرات]] لتسهيل تحديدها واكتشافها.

إذا كنت تريد إزالة الصفحة من قائمة المراقبة لديك، اضغط على "توقف عن المراقبة" بالأعلى.',
'removedwatch'         => 'تم الحذف من قائمة المراقبة',
'removedwatchtext'     => 'تم حذف الصفحة "$1" من قائمة مراقبتك.',
'watch'                => 'راقب',
'watchthispage'        => 'راقب هذه الصفحة',
'unwatch'              => 'توقف عن المراقبة',
'unwatchthispage'      => 'توقف عن مراقبة الصفحة',
'notanarticle'         => 'ليست صفحة محتوى',
'watchnochange'        => 'لم يتم تعديل أي صفحة في قائمة المراقبة لديك خلال الفترة المحددة.',
'watchdetails'         => 'لديك $1 صفحة في قائمة مراقبتك (بدون عد صفحات النقاش). يمكنك [[Special:Watchlist/edit|عرض و تعديل القائمة كاملة هنا]] أو [[Special:Watchlist/clear|إزالة جميع الصفحات]] من قائمة المراقبة.',
'wlheader-enotif'      => '* خاصية الإعلام بالبريد الإلكتروني ممكنة.',
'wlheader-showupdated' => "* الصفحات التي تمّ تحريرها بعد زيارتك لها لآخر مرة معروضة بالخط '''الغامق'''",
'watchmethod-recent'   => 'تفحص التغييرات الأخيرة في قائمة المراقة لديك',
'watchmethod-list'     => 'إظهار التحريرات في الصفحات المراقبة',
'removechecked'        => 'حذف المواد المختارة من قائمة المراقبة.',
'watchlistcontains'    => 'تحتوي قائمة المراقبة لديك على $1 صفحة.',
'watcheditlist'        => "فيما يلي قائمة مرتبة أبجديا للصفحات الموجودة في قائمة المراقبة لديك.
أختر الصفحات التري تريد إزالتها من خلال الإشارة عليها من الصندوق بجانبها.
وإضغط على زر 'حذف المختارات' في آخر الصفحة.",
'removingchecked'      => 'حذف الصفحات المطلوبة من قائمة المراقبة...',
'couldntremove'        => "لم ينجح مسح '$1'...",
'iteminvalidname'      => "مشكلة في المدخل '$1', اسم غير صحيح...",
'wlnote'               => 'في الأسفل آخر $1 تعديل في آخر <b>$2</b> ساعة.',
'wlshowlast'           => 'عرض آخر $1 ساعات $2 أيام $3',
'wlsaved'              => 'هذه نسخة مخزنة من قائمة المراقبة لديك.',
'watchlist-show-bots'  => 'عرض تعديلات البوتات',
'watchlist-hide-bots'  => 'إخفاء تعديلات البوتات',
'watchlist-show-own'   => 'عرض تعديلاتي',
'watchlist-hide-own'   => 'إخفاء تعديلاتي',
'watchlist-show-minor' => 'عرض التعديلات الطفيفة',
'watchlist-hide-minor' => 'إخفاء التعديلات الطفيفة',
'wldone'               => 'تم بنجاح.',

# Displayed when you click the "watch" button and it's in the process of watching
'watching'   => 'مراقبة...',
'unwatching' => 'إزالة المراقبة...',

'enotif_mailer'      => 'نظام {{SITENAME}} البريدي للملاحظات',
'enotif_reset'       => 'اعتبر كل الصفحات تم زيارتها',
'enotif_newpagetext' => 'هذه صفحة جديدة.',
'changed'            => 'تم التغيير',
'created'            => 'أنشأ',
'enotif_subject'     => 'صفحة {{SITENAME}} $PAGETITLE تم $CHANGEDORCREATED بواسطة $PAGEEDITOR',
'enotif_lastvisited' => 'انظر $1 لمراجعة كل التغييرات منذ زيارتك الأخيرة.',
'enotif_body'        => 'عزيزي $WATCHINGUSERNAME،

إن صفحة $PAGETITLE في {{SITENAME}} قد تغيرت إلى $CHANGEDORCREATED من قبل $PAGEEDITOR، لرؤية النسخة الحالية من الصفحة انظر هنا: $PAGETITLE_URL.

$NEWPAGE

ملخص التحرير: $PAGESUMMARY $PAGEMINOREDIT

اتصل بالمستخدم المحرر:
بريد إلكتروني: $PAGEEDITOR_EMAIL
صفحة المستخدم: $PAGEEDITOR_WIKI

لن يكون هنالك أي إخطارات أخرى عن أية تغييرات تتم في المستقبل إلا إذا زرت هذه الصفحة. يمكنك أيضا أن تعيد تصفير عداد الإخطارات لجميع الصفحات التي تراقبها في قائمة مراقبتك.

رسالة إرشادية من {{SITENAME}}

--
لتغيير قائمة مراقبتك اذهب إلى:
{{fullurl:{{ns:special}}:Watchlist/edit}}

للاقتراحات والحصول على مساعدة إضافية:
{{fullurl:{{MediaWiki:helppage}}}}',

# Delete/protect/revert
'deletepage'                  => 'احذف الصفحة',
'confirm'                     => 'تأكيد',
'excontent'                   => "المحتوى كان: '$1'",
'excontentauthor'             => "المحتوى كان: '$1' (و المساهم الوحيد كان '$2')",
'exbeforeblank'               => "المحتوى قبل الحذف كان: '$1'",
'exblank'                     => 'الصفحة كانت فارغة',
'confirmdelete'               => 'تأكيد الحذف',
'deletesub'                   => '(حذف "$1")',
'historywarning'              => 'تحذير: هذه الصفحة التي توشك على حذفها تحتوي على تغييرات سابقة:',
'confirmdeletetext'           => 'أنت على وشك أن تقوم بحذف صفحة أو صورة بالإضافة إلى جميع التعديلات عليها بشكل دائم من قاعدة البيانات. الرجاء التأكد من عزمك على الحذف وبأنك مدرك لنتائج هذه العملية. عمليات الحذف يجب أن تتم بناءً على [[Project:حذف الصفحات|القواعد المتفق عليها]].',
'actioncomplete'              => 'انتهاء العملية',
'deletedtext'                 => '"$1" تم حذفها.
انظر في $2 لسجل آخر عمليات الحذف.',
'deletedarticle'              => 'تم حذف "$1"',
'dellogpage'                  => 'سجل_الحذف',
'dellogpagetext'              => 'قائمة بأحدث الصفحات المحذوفة.',
'deletionlog'                 => 'سجل الحذف',
'reverted'                    => 'استرجع لنسخة أقدم',
'deletecomment'               => 'سبب الحذف',
'imagereverted'               => 'تم الرجوع إلى نسخة أقدم.',
'rollback'                    => 'استرجاع التعديلات',
'rollback_short'              => 'استرجاع',
'rollbacklink'                => 'استرجاع',
'rollbackfailed'              => 'لم ينجح الاسترجاع',
'cantrollback'                => 'لم ينجح الاسترجاع فآخر تعديل للصفحة كان من قبل المستخدم الوحيد الذي عدّل على هذه الصفحة.',
'alreadyrolled'               => 'لم ينجح استرجاع آخر تعديل لصفحة [[$1]] من قبل [[User:$2|$2]] ([[User talk:$2|نقاش]]) لأن هنالك من قام بتعديل أو استرجاع هذه الصفحة من قبل.

آخر تعديل كان بواسطة [[User:$3|$3]] ([[User talk:$3|نقاش]]).',
'editcomment'                 => 'ملاحظة التحرير كانت: "<i>$1</i>".', # only shown if there is an edit comment
'revertpage'                  => 'تم استرجاع تعديلات [[Special:Contributions/$2|$2]] ([[User talk:$2|نقاش]]); تم الاسترجاع حتى تعديل [[User:$1|$1]]',
'sessionfailure'              => 'يبدو أن هنالك مشكلة في هذه الجلسة الخاصة بك، لذلك فقد ألغيت هذه العملية كإجراء احترازي ضد الاختراق. الرجاء الضغط على مفتاح "العودة" (Back) للرجوع للصفحة التي جئت منها ثم قمّ بإعادة تحميلها قبل المحاولة مرة أخرى.',
'protectlogpage'              => 'سجل_الحماية',
'protectlogtext'              => 'هذه قائمة بالصفحات التي تمّ حمايتها أو إزالة الحماية عنها. للمزيد من المعلومات انظر [[Project:Protected page|قائمة الصفحات المحمية]].',
'protectedarticle'            => 'حماية [[$1]]',
'unprotectedarticle'          => 'ازالة حماية [[$1]]',
'protectsub'                  => '(حماية "$1")',
'confirmprotecttext'          => 'هل أنت متأكد انك تريد حماية هذه الصفحة؟',
'confirmprotect'              => 'تأكيد الحماية',
'protectmoveonly'             => 'الحماية من النقل فقط',
'protectcomment'              => 'سبب الحماية',
'protectexpiry'               => 'تنتهي في',
'protect_expiry_invalid'      => 'وقت إنتهاء المنع غير صحيح.',
'protect_expiry_old'          => 'وقت إنتهاء المنع يقع في الماضي.',
'unprotectsub'                => '(إزالة حماية "$1")',
'confirmunprotecttext'        => 'هل أنت متأكد انك تريد إزالة الحماية عن هذه الصفحة؟',
'confirmunprotect'            => 'تأكيد إزالة الحماية',
'unprotectcomment'            => 'سبب إزالة الحماية',
'protect-unchain'             => 'اسمح بنقل المقالة',
'protect-text'                => 'يمكنك هنا أن تعرض و تغير مستوى الحماية للصفحة <strong>$1</strong>.',
'protect-locked-blocked'      => 'لا يمكنك تغيير مستويات الحماية وأنت ممنوع.
الإعدادات الحالية للصفحة <strong>$1</strong> هي:',
'protect-locked-dblock'       => 'لا يمكن تغيير مستويات الحماية بسبب غلق قاعدة البيانات حاليا.
الإعدادات الحالية للصفحة <strong>$1</strong> هي:',
'protect-locked-access'       => 'لا يملك حسابك هذا صلاحية تغيير مستوى حماية الصفحة.
الإعدادات الحالية للصفحة <strong>$1</strong> هي:',
'protect-cascadeon'           => 'هذه الصفحة محمية لكونها مضمنة في الصفحات التالية، والتي بها خيار حماية الصفحات المدمجة فعال. يمكنك تغيير مستوى حماية هذه الصفحة بدون التأثير على حماية الصفحات المدمجة الأخرى.',
'protect-default'             => 'السماح لجميع المستخدمين.',
'protect-level-autoconfirmed' => 'منع المستخدمين غير المسجلين.',
'protect-level-sysop'         => 'منع جميع المستخدمين (عدا الإداريين).',
'protect-summary-cascade'     => 'حماية الصفحات المدمجة بهذه الصفخة',
'protect-expiring'            => 'ينتهي في $1 (UTC)',
'protect-cascade'             => 'حماية الصفحات المدمجة بهذه الصفحة.',
'restriction-type'            => 'صلاحية',
'restriction-level'           => 'مستوى الضوابط',
'minimum-size'                => 'الحجم الأدنى (بايت)',

# Restrictions (nouns)
'restriction-edit' => 'تعديل',
'restriction-move' => 'نقل',

# Restriction levels
'restriction-level-sysop'         => 'حماية كاملة',
'restriction-level-autoconfirmed' => 'حماية جزئية',
'restriction-level-all'           => 'أي مستوى',

# Undelete
'undelete'                 => 'إرجاع صفحات محذوفة',
'undeletepage'             => 'استعراض واسترجاع الصفحات المحذوفة',
'viewdeletedpage'          => 'عرض الصفحات المحذوفة',
'undeletepagetext'         => 'تم إلغاء الصفحات التالية و لكنها مازلت في الأرشيف و يمكن استرجاعها. يمكن مسح الأرشيف بشكل دوري.',
'undeleteextrahelp'        => "لاسترجاع الصفحة كاملة، اترك جميع الصناديق فارغة
و اضغط '''''استرجاع'''''. للاسترجاع بشكل انتقائي، ضع علامة في الصناديق أمام التعديلات التي تريد استرجاعها، و اضغط '''''استرجاع'''''. الضغط على '''''إلغاء''''' سيؤدي إلى إفراغ التعليق و حذف جميع العلامات من الصناديق.",
'undeleterevisions'        => '$1 نسخة تم أرشفتها',
'undeletehistory'          => 'استرجاع هذه الصفحة يؤدي إلى استرجاع جميع التغييرات المخزنة في تاريخ الصفحة. إذا كان هنالك صفحة جديدة تحمل نفس الاسم، فإن التغييرات القديمة سوف تدمج في تاريخ الصفحة الجديدة.',
'undeletehistorynoadmin'   => 'لقد تمّ حذف هذه الصفحة للأسباب المدرجة في الأسفل. إلى جانب أسباب الحذف يمكن رؤية المستخدمين الذين قاموا بالتعديل على هذه الصفحة قبل حذفها. بإمكان الإداريين فقط رؤية المحتويات الأصلية للمقال والتعديلات عليها.',
'undeletebtn'              => 'استرجاع!',
'undeletereset'            => 'إلغاء',
'undeletecomment'          => 'تعليق:',
'undeletedarticle'         => 'تم استرجاع "[[$1]]"',
'undeletedrevisions'       => 'تم استرجاع {{PLURAL:$1|تعديل واحد|تعديلان|$1 تعديلات|$1 تعديل|$1 تعديلا}}',
'undeletedrevisions-files' => '$1 مراجعة و $2 ملف تم استرجاعها',
'undeletedfiles'           => '$1 استرجاع ملفات',
'cannotundelete'           => 'فشل الاسترجاع؛ ربما قام شخص آخر باسترجاع الصفحة بالفعل.',
'undeletedpage'            => "<big>'''تم استرجاع $1'''</big>

راجع [[Special:Log/delete|سجل الحدف]] لمعاينة عمليات الحذف و الاسترجاعات الحديثة.",
'undelete-header'          => 'أنظر الصفحات المحذوفة حديثا في [[Special:Log/delete|سجل الحذف]].',
'undelete-search-box'      => 'ابحث في الصفحات المحذوفة',
'undelete-search-prefix'   => 'عرض الصفحات التي تبدأ بـ:',
'undelete-search-submit'   => 'ابحث',
'undelete-no-results'      => 'لم يتم العثور على صفحات بهذه المواصفات في أرشيف المحذوفات',

# Namespace form on various pages
'namespace' => 'النطاق:',
'invert'    => 'عكس الإختيار',

# Contributions
'contributions' => 'مساهمات المستخدم',
'mycontris'     => 'مساهماتي',
'contribsub2'    => 'للمستخدم $1 ($2)',
'nocontribs'    => 'لا توجد تعديلات تطابق هذه المحددات.',
'ucnote'        => 'في الأسفل ستجد آخر <b>$1</b> تعديل لهذا المستخدم في <b>$2</b> أيام.',
'uclinks'       => 'عرض آخر $1 تعديل;  عرض آخر $2 يوم.',
'uctop'         => ' (أعلى)',

'sp-contributions-newest'      => 'آخر',
'sp-contributions-oldest'      => 'أول',
'sp-contributions-newer'       => '$1 التالية',
'sp-contributions-older'       => '$1 السابقة',
'sp-contributions-newbies'     => 'عرض فقط المساهمات للحسابات الجديدة',
'sp-contributions-newbies-sub' => 'للمبتدئين',
'sp-contributions-blocklog'    => 'سجل المنع',
'sp-contributions-search'      => 'بحث عن مساهمات',
'sp-contributions-username'    => 'عنوان أيبي أو اسم مستخدم:',
'sp-contributions-submit'      => 'بحث',

'sp-newimages-showfrom' => 'أظهر الصور الجديدة انطلاقا من $1',

# What links here
'whatlinkshere'      => 'ماذا يرتبط هنا؟',
'notargettitle'      => 'لا هدف',
'notargettext'       => 'لم تحدد الصفحة او المستخدم المستهدف
لعمل هذه الخاصية.',
'linklistsub'        => '(قائمة الوصلات)',
'linkshere'          => "الصفحات التالية تحتوي على وصلة إلى '''[[:$1]]''':",
'nolinkshere'        => "لا يوجد صفحات تصل إلى '''[[:$1]]'''.",
'nolinkshere-ns'     => "لا تصل أي صفحة إلى '''[[:$1]]''' في النطاق المختار.",
'isredirect'         => 'صفحة تحويل',
'istemplate'         => 'مضمن',
'whatlinkshere-prev' => '{{PLURAL:$1|السابق|الـ $1 السابقة}}',
'whatlinkshere-next' => '{{PLURAL:$1|القادمة|الـ $1 القادمة}}',

# Block/unblock
'blockip'                     => 'منع مستخدم',
'blockiptext'                 => 'استخدم الاستمارة أدناه لمنع عنوان أيبي
أو مستخدم مسجل من الكتابة.
يجب أن يتم هذا فقط لمنع التخريب ومتوافقا مع
[[{{MediaWiki:policy-url}}|السياسة]] المتبعة هنا.
قم بذكر السبب أدناه (كمثال، اذكر بعض الصفحات التي قام بتخريبها تحديدا).',
'ipaddress'                   => 'عنوان الأي بي',
'ipadressorusername'          => 'عنوان المستخدم (IP) أو اسمه',
'ipbexpiry'                   => 'مدّة المنع',
'ipbreason'                   => 'السبب',
'ipbreasonotherlist'          => 'سبب آخر',
'ipbreason-list'              => '*#أسباب المنع عناوين الأيبي
*تخريب
*سخام وصلات
*#أسباب المنع للمستخدمين المسجلين
*سباب
*دمية جورب',
'ipbanononly'                 => 'فقط المستخدمين المجهولين',
'ipbcreateaccount'            => 'منع إنشاء حساب جديد',
'ipbenableautoblock'          => 'امنع العناوين المستخدمة بواسطة هذا المستخدم.',
'ipbsubmit'                   => 'امنع هذا المستخدم',
'ipbother'                    => "مدّة المنع (عن اختيار ''غير ذلك'')",
'ipboptions'                  => 'ربع ساعة:15 minutes,ساعة كاملة:1 hour,ساعتين:2 hours,يوم:1 day,ثلاثة أيام:3 days,أسبوع:1 week,أسبوعان:2 weeks,شهر:1 month,ثلاثة شهور:3 months,ستة شهور:6 months,عام كامل:1 year,دائم:infinite',
'ipbotheroption'              => 'غير ذلك',
'ipbotherreason'              => 'سبب إضافي',
'ipbhidename'                 => 'إخفاء اسم/عنوان المستخدم من سجل المنع وسجل المنع الحالي وقائمة المستخدمين',
'badipaddress'                => 'عنوان أيبي خاطئ',
'blockipsuccesssub'           => 'تم المنع بنجاح.',
'blockipsuccesstext'          => 'تم منع [[Special:Contributions/$1|$1]].
<br />أنظر [[Special:Ipblocklist|IP block list]] لمراجعة حالات المنع.',
'ipb-unblock-addr'            => 'إزالة منع $1',
'ipb-unblock'                 => 'رفع المنع عن المستخدم أو عنوان اللآي بي',
'ipb-blocklist-addr'          => 'عرض المنع الحالي لـ $1',
'ipb-blocklist'               => 'عرض حالات المنع الحالية',
'unblockip'                   => 'إزالة منع مستخدم',
'unblockiptext'               => 'استخدم الحقل أدناه لاسترجاع صلاحية الكتابة
الخاصة بIP أو مستخدم تم سحبها منه مسبقا.',
'ipusubmit'                   => 'إزالة المنع',
'unblocked'                   => 'تم منع [[مستخدم:$1|$1]]',
'ipblocklist'                 => 'قائمة أسماء الأعضاء و عناوين الأيبي الممنوعة',
'ipblocklist-submit'          => 'بحث',
'blocklistline'               => '$1, $2 منع $3 ($4)',
'infiniteblock'               => 'غير محدد',
'expiringblock'               => 'ينتهي في $1',
'anononlyblock'               => 'مجهول فقط',
'noautoblockblock'            => 'المنع التلقائي معطل',
'createaccountblock'          => 'تم منع إنشاء الحسابات.',
'ipblocklistempty'            => 'قائمة المنع فارغة.',
'blocklink'                   => 'منع مستخدم',
'unblocklink'                 => 'رفع المنع عن مستخدم',
'contribslink'                => 'مساهمات',
'autoblocker'                 => 'تم منعك تلقائيا لإن الأيبي الخاص بك تم استخدامه مؤخرا بواسطة المستخدم "[[مستخدم:$1|$1]]". سبب منع $1 هو: "\'\'\'$2\'\'\'"',
'blocklogpage'                => 'سجل_المنع',
'blocklogentry'               => 'منع "[[$1]]" لفترة زمنية مدتها $2 $3',
'blocklogtext'                => 'السجل التالي يبين عمليات منع التحرير وإزالة المنع عن المستخدمين. عناوين الأي بي التي تم منعها تلقائيا ليست ضمن هذه القائمة. انظر [[Special:Ipblocklist|عناوين الأي بي الممنوعة]] لرؤية العناوين الممنوعة حاليا.',
'unblocklogentry'             => 'إزالة منع $1',
'block-log-flags-anononly'    => 'المستخدمون المجهولون فقط',
'block-log-flags-nocreate'    => 'منع إنشاء حساب',
'block-log-flags-autoblock'   => 'منع تلقائي',
'range_block_disabled'        => 'إمكانية الإداري لمنع نطاق معطلة.',
'ipb_expiry_invalid'          => 'تاريخ انتهاء المنع المدخل غير صحيح.',
'ipb_already_blocked'         => 'سبق منع "$1"',
'ip_range_invalid'            => 'نطاق العناوين المدخل (IP) غير صحيح.',
'proxyblocker'                => 'مانع الوكيل (البروكسي)',
'ipb_cant_unblock'            => 'خطأ: لم يتم إيجاد الممنوع $1. ربما تم إلغاء منعه بالفعل.',
'proxyblockreason'            => 'تم منع عنوان الأيبي الخاص بك لكونه وكيل (بروكسي) مفتوح. قم بمراسلة مزود خدمة الإنترنت الخاص بك أو الدعم الفني وأعلمهم بهذه المشكلة الأمنية.',
'proxyblocksuccess'           => 'تم الأمر.',
'sorbsreason'                 => 'هذا الIP تم إدراجه كبروكسي مفتوح في [http://www.sorbs.net SORBS] DNSBL.',
'sorbs_create_account_reason' => 'هذا الIP تم إدراجه كبروكسي مفتوح في [http://www.sorbs.net SORBS] DNSBL. لا يمكنك إنشاء هذا الحساب',

# Developer tools
'lockdb'              => 'أقفل قاعدة البيانات',
'unlockdb'            => 'افتح قاعدة البيانات',
'lockdbtext'          => 'إغلاق قاعدة البيانات سوف يمنع جميع المستخدمين من تحرير الصفحات وتغيير التفضيلات الخاصة بهم وتعديل قائمة المراقبة وغير ذلك من الأمور المتطلبة التعديل على قاعدة البيانات. الرجاء التأكد من أن هذا حقا ما ترغب القيام به، ومن أنك سوف تقوم بإزالة الغلق بعد الانتهاء من الصيانة.',
'unlockdbtext'        => 'فتح قاعدة البيانات سيسترجع قدرة كل المستخدمين على تحرير الصفحات, تغيير تفضيلاتهم
, تعديل قوائم المراقبة الخاصة بهم, و
أشياء أخرى تحتاج التغيير في قاعدة البيانات.
من فضلك, أكد أن هذا هو ما تريده.',
'lockconfirm'         => 'نعم، أنا متأكد أني أريد قفل قاعدة البيانات.',
'unlockconfirm'       => 'نعم, أريد فعلا فتح قاعدة البيانات.',
'lockbtn'             => 'أقفل قاعدة البيانات',
'unlockbtn'           => 'فتح قاعدة البيانات',
'locknoconfirm'       => 'لم تقم باختيار خانة التأكيد.',
'lockdbsuccesssub'    => 'نجح إقفال قاعدة البيانات',
'unlockdbsuccesssub'  => 'تم إزالة قفل قاعدة البيانات',
'lockdbsuccesstext'   => 'لقد أغلقت قاعدة البيانات.
<br />تذكر بأن تزيل الغلق بعد اكتمال أعمال الصيانة.',
'unlockdbsuccesstext' => 'تم إعادة فتح قاعدة البيانات',
'lockfilenotwritable' => 'ملف غلق قاعدة البيانات لا يمكن الكتابة عليه. لغلق قاعدة البيانات أو إزالة الغلق يجب أن يكون هذا الملف قابلا للكتابة من قبل الخادم.',
'databasenotlocked'   => 'قاعدة البيانات ليست مغلقة.',

# Move page
'movepage'                => 'نقل صفحة',
'movepagetext'            => "باستعمال النموذج أدناه بإمكانك أن تغيّر اسم الصفحة، وأن تنقل تاريخها للاسم الجديد.
سيتم إنشاء تحويل من العنوان القديم للصفحة بالعنوان الجديد. لكن، لن يتم تغيير الوصلات في الصفحات التي تتصل بهذه الصفحة، لذا عليك [[Special:Maintenance|التأكد]] من عدم وجود وصلات مقطوعة، أو وصلات متتالية، للتأكد من أن المقالات تتصل مع بعضها بشكل مناسب.

يرجى الملاحظة أنه '''لن يتمّ''' نقل الصفحة إذا وجدت صفحة بالاسم الجديد، إلا إذا كانت صفحة فارغة، أو صفحة تحويل، ولا تاريخ لها. وهذا يعني أنك لا تستطيع وضع صفحة مكان صفحة، كما أنه من الممكن ارجاع الصفحة لمكانها في حال تم النقل بشكل خاطئ.

<b>تحذير!</b> قد يكون لنقل الصفحة آثار كبيرة، وتغييرا غير متوقع بالنسبة للصفحات المشهورة. يرجى فهم وإدارك عواقب نقل الصفحات قبل القيام به.",
'movepagetalktext'        => 'صفحة النقاش المرفقة بالمقالة سيتم نقلها كذلك، إذا وجدت. ولكن لا يتم نقل صفحة النقاش في الحالات التالية:
* نقل الصفحة عبر نطاقات namespaces مختلفة.
* يوجد صفحة نقاش غير فارغة تحت العنوان الجديد للمقالة.
* قمت بإزالة إختيار نقل صفحة النقاش في الأسفل.

وفي الحالات أعلاه، يجب عليك نقل أو دمج محتويات صفحة النقاش يدويا، إذا رغب في ذلك.',
'movearticle'             => 'انقل الصفحة',
'movenologin'             => 'غير مسجل',
'movenologintext'         => 'يجب أن تكون متسخدمًا مسجلاً وأن تقوم [[Special:Userlogin|بالدخول]] لكي تنقل صفحة.',
'newtitle'                => 'إلى العنوان الجديد',
'move-watch'              => 'راقب هذه الصفحة',
'movepagebtn'             => 'أنقل الصفحة',
'pagemovedsub'            => 'تم النقل بنجاح',
'pagemovedtext'           => 'تم نقل الصفحة "[[$1]]" إلى "[[$2]]".',
'articleexists'           => 'يوجد صفحة بهذا الاسم،
أو أن الاسم الذي تم إختياره غير صالح.
يرجى إختيار اسم آخر.',
'talkexists'              => 'تم نقل الصفحة بنجاح، لكن لم
يتم نقل صفحة النقاش المرافقة، بسبب وجود صفحة نقاش
مسبقا تحت العوان الجديد.
يرجى نقل محتويات صفحة النقاش يدويا، ودمجها مع المحتويات السابقة.',
'movedto'                 => 'تم نقلها إلى',
'movetalk'                => "أنقل صفحة '''النقاش''' أن أمكن.",
'talkpagemoved'           => 'تم نقل صفحة النقاش أيضا.',
'talkpagenotmoved'        => '<strong>لم</strong> يتم نقل صفحة النقاش.',
'1movedto2'               => '$1 تم نقلها إلى $2',
'1movedto2_redir'         => 'تم نقل $1 فوق التحويلة $2',
'movelogpage'             => 'سجل النقل',
'movelogpagetext'         => 'هذه قائمة بالصفحات التي تمّ نقلها.',
'movereason'              => 'السبب',
'revertmove'              => 'استرجاع',
'delete_and_move'         => 'احذف وانقل',
'delete_and_move_text'    => '==يستلزم الحذف==
هناك مقالة تحت الاسم - [[$1]] - الذي طلبت أن تنقل إليه هذه المقالة. هل ترغب بحذف تلك المقالة لإتمام عملية النقل؟',
'delete_and_move_confirm' => 'نعم، احذف هذه الصفحة.',
'delete_and_move_reason'  => 'حذف لإفساح مجال للنقل',
'selfmove'                => 'لا يوجد اختلاف في عنوان المصدر و الهدف ، لا يمكن نقل الصفحة على نفسها.',
'immobile_namespace'      => 'لا يمكن النقل إلى العنوان المراد، النطاق غير مسموح بالنقل إليه.',

# Export
'export'            => 'صدّر صفحات',
'exporttext'        => 'يمكنك تصدير النص و تاريخ تعديلات صفحة أو مجموعة صفحات خاصة في صيغة إكس.إم.إل. وذلك بقصد استيرادها في ويكي آخر يستعمل ميدياويكي بواسطة الصفحة [[Special:Import|صفحة الاستيراد]].

لتصدير صفحات، اكتب العناوين في الصندوق أسفله، عنوان واحد في كل السطر، مع اختيار ما إذا كنت ترغب في النسخة الحالية بالإضافة إلى النسخ القديمة كاملة أو مع معلومات تاريخ الصفحة عنها أو فقط النسخة الحالية مع معلومات عن التعديل الأخير.

في الحالة الأخيرة يمكنك استعمال وصلة مباشرة، كمثال [[{{ns:Special}}:Export/{{Mediawiki:mainpage}}]] للصفحة {{Mediawiki:mainpage}}.',
'exportcuronly'     => 'ضمن النسخة الحالية فقط ، وليس تاريخ الصفحة بأكمله.',
'exportnohistory'   => "----ملاحظة:''' تم توقيف التصدير الكامل لتاريخ الصفحة بهذه الترسيمة لأسباب تحسينية'''",
'export-submit'     => 'تصدير',
'export-addcattext' => 'إضافة صفحات من تصنيف:',
'export-addcat'     => 'إضافة',

# Namespace 8 related
'allmessages'               => 'كافة رسائل النظام',
'allmessagesname'           => 'الاسم',
'allmessagesdefault'        => 'النص الافتراضي',
'allmessagescurrent'        => 'النص الحالي',
'allmessagestext'           => 'هذه قائمة بكافة رسائل النظام المتوفرة في نطاق ميدياويكي.',
'allmessagesnotsupportedUI' => 'لغة الواجهة الحالية الخاصة بك <b>$1</b> غير مدعمة في {{ns:special}}:Allmessages في هذا الموقع.',
'allmessagesnotsupportedDB' => "لا يمكن استخدام '''{{ns:special}}:Allmessages''' لأن المتغير wgUseDatabaseMessages غير مشغل.",
'allmessagesfilter'         => 'بحث باسم الرسالة:',
'allmessagesmodified'       => 'إظهر المتغير فقط',

# Thumbnails
'thumbnail-more'  => 'تكبير',
'missingimage'    => '<b>صورة غير موجودة</b><br /><i>$1</i>',
'filemissing'     => 'الملف مفقود',
'thumbnail_error' => 'خطأ في إنشاء صورة مصغرة:  $1',

# Special:Import
'import'                     => 'استيراد صفحات',
'importinterwiki'            => 'استيراد ويكي',
'import-interwiki-text'      => 'اختر الويكي و عنوان الصفحة المطلوب استيرادها.
تواريخ التعديلات و أسماء المحررين سيتم الحفاظ عليها.
كل عمليات الاستيراد عبر مواقع الويكي يتم تسجيلها في [[Special:Log/import|سجل الاستيراد]].',
'import-interwiki-history'   => 'انسخ تاريخ الصفحة كله',
'import-interwiki-submit'    => 'استيراد',
'import-interwiki-namespace' => 'انقل الصفحات إلى النطاق:',
'importtext'                 => 'من فضلك صدّر الملف من الويكي المصدر عن طريق Special:Export، احفظها على جهازك ثم ارفعها هنا.',
'importstart'                => 'استيراد صفحات...',
'import-revision-count'      => 'تعديل(ات) $1',
'importnopages'              => 'لا صفحات للاستيراد',
'importfailed'               => 'فشل استيراد: $1',
'importunknownsource'        => 'نوع مجهول لمصدر الاستيراد',
'importcantopen'             => 'لم يتمكن من فتح الملف المستورد',
'importbadinterwiki'         => 'وصلة إنترويكي خاطئة',
'importnotext'               => 'فارغ أو لا يحتوي على نص',
'importsuccess'              => 'تم الاستيراد بنجاح!',
'importhistoryconflict'      => 'هناك تاريخ تعديلات يتعارض مع تاريخ هذه الصفحة (ربما تكون قد استوردت الصفحة من قبل)',
'importnosources'            => 'لم يتم تحديد مصادر للاستيراد الويكي و الاستيراد المباشر عن طريق الرفع غير فعال.',
'importnofile'               => 'لم يتم رفع ملف استيراد.',
'importuploaderror'          => 'فشل رفع أو استيراد الملف، ربما الملف أكبر من الحجم المسموح به.',

# Import log
'importlogpage'                    => 'سجل الاستيراد',
'importlogpagetext'                => 'استيرادات إدارية (للإداريين فقط) لصفحات من مواقع ويكي أخرى.',
'import-logentry-upload'           => 'تم استيراد $1 عن طريق رفع ملف',
'import-logentry-upload-detail'    => 'تعديل(ات) $1',
'import-logentry-interwiki'        => 'استيراد ويكي $1',
'import-logentry-interwiki-detail' => 'تعديل(ات) من $2 $1',

# Tooltip help for the actions
'tooltip-pt-userpage'             => 'صفحتي الخاصة',
'tooltip-pt-anonuserpage'         => 'صفحة المستخدم لهذا الأيببي الذي تقوم بالتحرير من خلاله',
'tooltip-pt-mytalk'               => 'صفحة نقاشي',
'tooltip-pt-anontalk'             => 'نقاش حول تعديلات هذا المستخدم المجهول',
'tooltip-pt-preferences'          => 'تفضيلاتي',
'tooltip-pt-watchlist'            => 'قائمة بالصفحات التي تقوم بمراقبة التعديلات التي تحدث فيها',
'tooltip-pt-mycontris'            => 'قائمة بمساهماتي',
'tooltip-pt-login'                => 'من المفضل أن تقوم بتسجيل الدخول، هذا ليس إلزاميا.',
'tooltip-pt-anonlogin'            => 'من المفضل أن تقوم بتسجيل الدخول، هذا ليس إلزاميا.',
'tooltip-pt-logout'               => 'خروج',
'tooltip-ca-talk'                 => 'نقاش حول صفحة المحتوى',
'tooltip-ca-edit'                 => 'بإمكانك تعديل هذه الصفحة، برجاء استعمال زر عرض التعديلات قبل الحفظ',
'tooltip-ca-addsection'           => 'أضف تعليق إلى هذا النقاش',
'tooltip-ca-viewsource'           => 'هذه الصفحة محمية، يمكنك رؤية المصدر النصي لها.',
'tooltip-ca-history'              => 'النسخ السابقة من هذه الصفحة',
'tooltip-ca-protect'              => 'لحماية هذه الصفحة',
'tooltip-ca-delete'               => 'احذف هذه الصفحة',
'tooltip-ca-undelete'             => 'استرجاع التعديلات التي تمت على هذه الصفحة قبل حذفها',
'tooltip-ca-move'                 => 'انقل هذه الصفحة',
'tooltip-ca-watch'                => 'أضف هذه الصفحة إلى قائمة مراقبتي',
'tooltip-ca-unwatch'              => 'أزل هذه الصفحة من قائمة مراقبتك',
'tooltip-search'                  => 'ابحث في {{SITENAME}}',
'tooltip-p-logo'                  => 'الصفحة الرئيسية',
'tooltip-n-mainpage'              => 'مطالعة الصفحة الرئيسية',
'tooltip-n-portal'                => 'حول المشروع، ماذا يمكن أن تفعل، أين يمكن أن تجد ما تحتاجه',
'tooltip-n-currentevents'         => 'مطالعة سريعة لأهم الأخبار الحالية',
'tooltip-n-recentchanges'         => 'قائمة بأحدث التعديلات في الويكي',
'tooltip-n-randompage'            => 'اعرض ضفحة عشوائية',
'tooltip-n-help'                  => 'تحتاج مساعدة؟ طالع هذه الصفحة!',
'tooltip-n-sitesupport'           => 'قم بدعمنا',
'tooltip-t-whatlinkshere'         => 'قائمة بكل الصفحات التي تصل هنا',
'tooltip-t-recentchangeslinked'   => 'أحدث التغييرات في الصفحات الموجودة وصلاتها هنا.',
'tooltip-feed-rss'                => 'تلقيم أر.إس.إس لهذه الصفحة',
'tooltip-feed-atom'               => 'تلقيم أتوم لهذه الصفحة',
'tooltip-t-contributions'         => 'عرض قائمة مساهمات هذا المستخدم',
'tooltip-t-emailuser'             => 'إرسال رسالة لهذا المستخدم',
'tooltip-t-upload'                => 'رفع صور أو ملفات وسائط أخرى',
'tooltip-t-specialpages'          => 'قائمة بكل الصفحات الخاصة',
'tooltip-ca-nstab-main'           => 'عرض صفحة المحتوى',
'tooltip-ca-nstab-user'           => 'اعرض صفحة المستخدم',
'tooltip-ca-nstab-media'          => 'اعرض صفحة ملف الوسائط',
'tooltip-ca-nstab-special'        => 'هذه صفحة خاصة لا تستطيع أن تقوم بتعديلها',
'tooltip-ca-nstab-project'        => 'اعرض صفحة خاصة بالمشروع',
'tooltip-ca-nstab-image'          => 'اعرض صفحة الصورة',
'tooltip-ca-nstab-mediawiki'      => 'اعرض رسالة النظام',
'tooltip-ca-nstab-template'       => 'اعرض القالب',
'tooltip-ca-nstab-help'           => 'اعرض صفحة المساعدة',
'tooltip-ca-nstab-category'       => 'اعرض صفحة التصنيف',
'tooltip-minoredit'               => 'اعرض هذا التغيير كتغيير طفيف',
'tooltip-save'                    => 'احفظ تعديلاتك',
'tooltip-preview'                 => 'اعرض تعديلاتك, استخدم هذا من فضلك قبل الحفظ!',
'tooltip-diff'                    => 'شاهد ماذا تغير بالنص.',
'tooltip-compareselectedversions' => 'شاهد الفروقات بين النسختين المختارتين.',
'tooltip-watch'                   => 'أضف هذه الصفحة إلى قائمة مراقبتك',
'tooltip-recreate'                => 'أعد إنشاء الصفحة رغم كونها حذفت',

# Stylesheets
'common.css'   => '/** النظم المتراصة الموضوعة هنا سيتم تطبيقها على كل الواجهات  */',
'monobook.css' => '/* CSS placed here will affect users of the Monobook skin */',

# Scripts
'common.js'   => '/* الجافاسكريبت الموضوع هنا سيتم تحميله لكل المستخدمين مع كل تحميل للصفحة. */',
'monobook.js' => '/* Deprecated; use [[MediaWiki:common.js]] */',

# Metadata
'notacceptable' => 'لا يمكن لخادم الويكي تزويدك ببيانات بصيغة يستطيع عميلك قراءتها.',

# Attribution
'anonymous'        => 'مستخدم مجهول ل{{SITENAME}}',
'siteuser'         => 'مستخدم {{SITENAME}} $1',
'lastmodifiedatby' => 'آخر تعديل لهذه الصفحة كان في $2، $1 بواسطة $3.', # $1 date, $2 time, $3 user
'and'              => 'و',
'othercontribs'    => 'بناء على عمل $1.',
'others'           => 'أخرى',

# Spam protection
'spamprotectiontitle'    => 'مانع السخام',
'spamprotectiontext'     => 'تم منع حفظ الصفحة عن طريق مانع السخام. من المحتمل أن يكون هذا بسبب وصلة خارجية.',
'spamprotectionmatch'    => 'النص التالي هو ما نشط مانع السخام الخاص بنا: $1',
'subcategorycount'       => 'يوجد {{PLURAL:$1|تصنيف فرعي واحد|تصنيفان فرعيان|$1 تصنيفات فرعية|$1 تصنيف فرعي|$1 تصنيفا فرعيا}} في هذا التصنيف.',
'categoryarticlecount'   => 'يوجد {{PLURAL:$1|مقال واحد|مقالان|$1 مقالات|$1 مقال|$1 مقالا}} في هذا التصنيف.',
'category-media-count'   => 'يوجد {{PLURAL:$1|ملف واحد|$1 ملفات}} في هذا التصنيف.',
'listingcontinuesabbrev' => ' (متابعة)',
'spambot_username'       => 'تنظيف سخام ميدياويكي',
'spam_reverting'         => 'استرجاع آخر نسخة ليس بها وصلات إلى $1',

# Info page
'infosubtitle'   => 'معلومات الصفحة',
'numedits'       => 'عدد التحريرات (مقالة): $1',
'numtalkedits'   => 'عدد التعديلات (صفحة نقاش): $1',
'numwatchers'    => 'عدد المراقبين: $1',
'numauthors'     => 'عدد المحررين (مقالة): $1',
'numtalkauthors' => 'عدد المستخدمين المحررين للصفحة (صحفة نقاش): $1',

# Math options
'mw_math_png'    => 'دائمًا اعرض على هيئة PNG',
'mw_math_simple' => 'اعرض على هيئة HTML للصيغ البسيطة جدًا، وإلا فاعرض بهئية PNG',
'mw_math_html'   => 'اعرض بصيغة HTML إن أمكن، وإلا فاعرض كهيئة PNG',
'mw_math_source' => 'اعرض على هيئة TeX (للمتصفحات النصية)',
'mw_math_modern' => 'مستحسن للمتصفحات الحديثة',
'mw_math_mathml' => 'اعرض بصيغة MathML إن أمكن (تحت التجريب)',

# Patrolling
'markaspatrolleddiff'                 => 'تمت مراجعتها',
'markaspatrolledtext'                 => 'هذه المقالة تمت مراجعتها',
'markedaspatrolled'                   => 'تمت مراجعتها',
'markedaspatrolledtext'               => 'تم الإشارة إلى النسخة المختارة كمراجعة.',
'rcpatroldisabled'                    => 'مراجعة أحدث التغييرات معطلة',
'rcpatroldisabledtext'                => 'خاصية مراجعة أحدث التغييرات معطلة حاليا',
'markedaspatrollederror'              => 'لا يمكن الإشارة بالمراجعة',
'markedaspatrollederrortext'          => 'يجب عليك اختيار النسخة التي تريد أن تشير أنها مراجعة',
'markedaspatrollederror-noautopatrol' => 'لا يمكن السماح لك بالإشارة إلى تعديلاتك الشخصية كتعديلات تمت مراجعتها.',

# Patrol log
'patrol-log-page' => 'سجل المراجعة',
'patrol-log-line' => 'قام بمراحعة $1 من صفحة $2 $3',
'patrol-log-auto' => '(تلقائيا)',
'patrol-log-diff' => 'ن$1',

# Image deletion
'deletedrevision' => 'حذف التعديل القديم $1.',

# Browsing diffs
'previousdiff' => '→ الفرق السابق',
'nextdiff'     => 'الفرق اللاحق ←',

# Media information
'mediawarning'         => "'''تنبيه''': هذا الملف يحتوي على كود خبيث، يمكن عند تشغيله السيطرة على نظامك.<hr />",
'imagemaxsize'         => 'حجم الصورة في صفحة وصفها:',
'thumbsize'            => 'حجم العرض المصغر:',
'file-info'            => '(حجم الملف: $1, نوع الملف: $2)',
'file-info-size'       => '($1 × $2 بيكسل, حجم الملف: $3, نوع الملف: $4)',
'file-nohires'         => '<small>لا توجد دقة أعلى متوفرة.</small>',
'show-big-image'       => 'الصورة بدقة أعلى',
'show-big-image-thumb' => '<small>حجم هذا العرض: $1 × $2 بيكسل</small>',

'newimages'    => 'معرض الصور الجديدة',
'showhidebots' => '($1 بوت)',
'noimages'     => 'لا يوجد محتوى ليتم عرضه.',

'passwordtooshort' => 'كلمة السرّ التي اخترتها قصيرة. يجب أن لا يقل طول الكلمة عن $1 خانة.',

# Metadata
'metadata'          => 'معلومات الصورة (ميتا)',
'metadata-help'     => 'هذا الملف يحتوي معلومات إضافية، غالبا ما تكون أضيفت من قبل آلة التصوير الإلكترونية أو الماسح الضوئي المستخدم في تحميل الصورة إلى الحاسوب. إذا كان الملف قد عُدّل عما كان عليه عند رفع الصورة فإن  المعلومات الواردة هنا قد لا تعبر عن هذه الصورة المعدلة.',
'metadata-expand'   => 'إظهار معلومات إضافية',
'metadata-collapse' => 'إخفاء المعلومات الإضافية',

# EXIF tags
'exif-imagewidth'          => 'العرض',
'exif-imagelength'         => 'الطول',
'exif-orientation'         => 'التوجية',
'exif-imagedescription'    => 'عنوان الصورة',
'exif-make'                => 'منتج آلة التصوير',
'exif-model'               => 'نوع آلة التصوير',
'exif-software'            => 'البرمجيات المستخدمة',
'exif-artist'              => 'المؤلف',
'exif-copyright'           => 'مالك الحقوق المحفوظة',
'exif-usercomment'         => 'تعليقات المستخدم',
'exif-datetimeoriginal'    => 'تاريخ و وقت الإنتاج',
'exif-exposuretime'        => 'مدة التعرض',
'exif-exposuretime-format' => '$1 ثانية ($2)',
'exif-shutterspeedvalue'   => 'سرعة الغالق',
'exif-focallength'         => 'البعد البؤري للعدسة',
'exif-focallength-format'  => '$1 ملم',
'exif-customrendered'      => 'معالجة الصورة حسب الطّلب',

'exif-orientation-1' => 'وضعية طبيعية', # 0th row: top; 0th column: left

'exif-exposureprogram-2' => 'برنامج إعتيادي',

'exif-meteringmode-255' => 'غير ذلك',

'exif-focalplaneresolutionunit-2' => 'بوصة',

'exif-customrendered-0' => 'عملية طبيعية',
'exif-customrendered-1' => 'عملية حسب الطلب',

'exif-contrast-0' => 'وضعية طبيعية',

'exif-saturation-0' => 'وضعية طبيعية',

'exif-sharpness-0' => 'وضعية طبيعية',

# Pseudotags used for GPSSpeedRef and GPSDestDistanceRef
'exif-gpsspeed-k' => 'كيلومتر في الساعة',

# External editor support
'edit-externally'      => 'حرر هذا الملف باستخدام تطبيق خارجي',
'edit-externally-help' => 'إقرأ [http://meta.wikimedia.org/wiki/Help:External_editors تعليمات الإعداد] لمزيد من المعلومات.',

# 'all' in various places, this might be different for inflected languages
'recentchangesall' => 'الجميع',
'imagelistall'     => 'الجميع',
'watchlistall1'    => 'الجميع',
'watchlistall2'    => 'الجميع',
'namespacesall'    => 'الكل',

# E-mail address confirmation
'confirmemail'            => 'أكد العنوان الإلكتروني',
'confirmemail_noemail'    => 'ليس لديك بريد صحيح مسجل في [[{{ns:special}}:Preferences|تفضيلاتك]].',
'confirmemail_text'       => 'هذا الموقع يتطلب تأكيد عنوانك الإلكتروني قبل استعمال خصائص البريد الإلكتروني. اضغط على زر التفعيل بالأسفل ليتم إرسال رسالة تأكيد لعنوانك. ستتضمن الرسالة رمز تفعيل، اضغط على الوصلة التي تحتوي الرمز لتأكيد أن عنوانك الإلكتروني صحيح.',
'confirmemail_pending'    => "<div class=\"error\">
'''تم إرسال كود التأكيد بالفعل من قبل إلى بريدك الإلكتروني، إذا كان حسابك قد تم عمله حالا، ربما يجب أن تنتظر بضع دقائق قبل طلبك لكود آخر.'''
</div>",
'confirmemail_send'       => 'أرسل رمز التفعيل',
'confirmemail_sent'       => 'تم إرسال رسالة التأكيد، شكرا لك.',
'confirmemail_oncreate'   => 'تم إرسال كود تأكيد لعنوان بريدك الإلكتروني.
لا يشترط أن تستخدم الكود للدخول للموسوعة باسمك، ولكن يجب أن تقوم بإدخاله قبل
استخدامك أيا من خواص البريد الإلكتروني المستخدمة هنا في الويكي.',
'confirmemail_sendfailed' => 'لم نستطع إرسال رسالة التأكيد. من فضلك، تأكد من بريدك الإلكتروني.

الخطأ الذي حدث: $1',
'confirmemail_invalid'    => 'رمز تفعيل خاطئ. ربما انتهت فترة صلاحيته.',
'confirmemail_needlogin'  => 'يجب عليك $1 لتأكيد البريد الإلكتروني الخاص بك.',
'confirmemail_success'    => 'تم تأكيد بريدك الإلكتروني، بإمكانك الآن الدخول و التمتع بالويكي.',
'confirmemail_loggedin'   => 'تم تأكيد بريدك الإلكتروني، شكرا لك.',
'confirmemail_error'      => 'ثمة شئ خطأ حدث عند محاولة حفظ تأكيدك.',
'confirmemail_subject'    => 'رسالة تأكيد البريد من {{SITENAME}}',
'confirmemail_body'       => 'شخصا ما, من المحتمل ان يكون أنت من العنوان $1 , قام بتسجيل 
حساب بهذا العنوان الإلكتروني على {{SITENAME}}, الحساب هو:

"$2"

لتأكيد أن هذا الحساب يخصك حقا و تريد تفعيل خواص البريد الإلكتروني به على {{SITENAME}}, افتح هذه الوصلة بمتصفحك:

$3

لو لم يكن هذا * أنت * , لا تضغط هذه الوصلة. رمز التفيعل سينتهي في $4.',

# Inputbox extension, may be useful in other contexts as well
'tryexact'       => 'ابحث عن عنوان مطابق',
'searchfulltext' => 'ابحث في النص الكامل',
'createarticle'  => 'إنشاء مقالة',

# Scary transclusion
'scarytranscludetoolong' => '[عنوان طويل للغاية; معذرة]',

# Trackbacks
'trackbackbox'      => '<div id="mw_trackbacks">
المتابعات الخاصة بهذه المقالة:<br />
$1
</div>',
'trackbackremove'   => ' ([$1 حذف])',
'trackbacklink'     => 'متابعة',
'trackbackdeleteok' => 'تم بنجاح إلغاء المتابعة.',

# Delete conflict
'deletedwhileediting' => "'''<span style=\"color:red;\">تحذير:</span> لقد تمّ حذف هذه الصفحة بعد أن بدأت بتعديلها.'''",
'confirmrecreate'     => "لقد قام المستخدم [[مستخدم:$1|$1]] ([[نقاش المستخدم:$1|نقاش]]) بحذف هذه المقالة بعد أن بدأت بتحريرها للسبب التالي:
:''$2''
الرجاء التأكد من أنك تريد إعادة إنشاء هذه المقالة.",
'recreate'            => 'أعد الإنشاء',

'unit-pixel' => 'بك',

# HTML dump
'redirectingto' => 'جاري التحويل إلى [[$1]]...',

# action=purge
'confirm_purge'        => 'امسح كاش هذه الصفحة؟

$1',
'confirm_purge_button' => 'تابع',

'youhavenewmessagesmulti' => 'لديك رسائل جديدة على $1',

'searchcontaining' => "ابحث عن المقالات التي تحتوي ''$1''.",
'searchnamed'      => "ابحث عن المقالات التي عنوانها ''$1''.",
'articletitles'    => "المقالات التي تبدأ ب ''$1''",
'hideresults'      => 'إخفاء النتائج',

# DISPLAYTITLE
'displaytitle' => '(اعرض وصلة هذه الصفحة هكذا [[$1]])',

'loginlanguagelabel' => 'اللغة: $1',

# Multipage image navigation
'imgmultipageprev'   => '&rarr; الصفحة السابقة',
'imgmultipagenext'   => 'الصفحة التالية &larr;',
'imgmultigo'         => 'اذهب!',
'imgmultigotopre'    => 'اذهب إلى صفحة',
'imgmultiparseerror' => 'يبدو أن ملف الصورة به خطأ ما أو غير صحيح، لذا لا يمكن أن تعرض {{SITENAME}} قائمة بالملفات.',

# Table pager
'ascending_abbrev'         => 'تصاعدي',
'descending_abbrev'        => 'تنازلي',
'table_pager_next'         => 'الصفحة التالية',
'table_pager_prev'         => 'الصفحة السابقة',
'table_pager_first'        => 'الصفحة الأولى',
'table_pager_last'         => 'الصفحة الأخيرة',
'table_pager_limit'        => 'اعرض $1 عنصر في الصفحة',
'table_pager_limit_submit' => 'اذهب',
'table_pager_empty'        => 'لا نتائج',

# Auto-summaries
'autosumm-blank'   => 'يرجى الانتباه إلى أن هذه الصفحة تم إفراغها',
'autosumm-replace' => "يرجى الانتباه! استبدل المحتوى بـ '$1'",
'autoredircomment' => 'تحويل إلى [[$1]]', # This should be changed to the new naming convention, but existed beforehand
'autosumm-new'     => 'صفحة جديدة: $1',

# Size units
'size-bytes'     => '$1 بايت',
'size-kilobytes' => '$1 كيلوبايت',
'size-megabytes' => '$1 ميجابايت',
'size-gigabytes' => '$1 جيجابايت',

# Live preview
'livepreview-loading' => 'جاري التحميل…',
'livepreview-ready'   => 'جاري التحميل… جاهز!',
'livepreview-failed'  => 'فشل العرض المباشر!
حاول تجربة العرض العادي.',
'livepreview-error'   => 'لم ينجح الاتصال: $1 "$2"
حاول تجربة العرض العادي.',

);

?>
