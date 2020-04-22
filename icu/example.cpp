#include <stdio.h>
#include <iostream>

#include "unicode/unistr.h"
#include "unicode/utypes.h"
#include "unicode/locid.h"
#include "unicode/coll.h"
#include "unicode/tblcoll.h"
#include "unicode/coleitr.h"
#include "unicode/sortkey.h"
#include "unicode/udata.h"
#include "unicode/timezone.h"
#include "unicode/calendar.h"
#include "unicode/datefmt.h"
using namespace icu;

UBool collateWithLocaleInCPP(const Locale& locale, UErrorCode& status)
{
	UnicodeString dispName;
	UnicodeString source("This is a test.");
	UnicodeString target("THIS IS A TEST.");
	Collator::EComparisonResult result    = Collator::EQUAL;
	CollationKey sourceKey;
	CollationKey targetKey;
	Collator      *myCollator = 0;
	if (U_FAILURE(status))
	{
		return FALSE;
	}
	myCollator = Collator::createInstance(locale, status);
	if (U_FAILURE(status)){
		locale.getDisplayName(dispName);
		/*Report the error with display name... */
		fprintf(stderr, "Failed to create the collator");
		return FALSE;
	}
	result = myCollator->compare(source, target);
	/* result is 1, secondary differences only for ignorable space characters*/
	if (result != Collator::EComparisonResult::LESS)
	{
		fprintf(stderr,
		"Comparing two strings with only secondary differences in C failed.\n");
		return FALSE;
	}
	/* To compare them with just primary differences */
	myCollator->setStrength(Collator::PRIMARY);
	result = myCollator->compare(source, target);
	/* result is 0 */
	if (result != 0)
	{
		fprintf(stderr,
		"Comparing two strings with no differences in C failed.\n");
		return FALSE;
	}
	/* Now, do the same comparison with keys */
	myCollator->getCollationKey(source, sourceKey, status);
	myCollator->getCollationKey(target, targetKey, status);
	result = Collator::EQUAL;

	result = sourceKey.compareTo(targetKey);
	if (result != 0)
	{
		fprintf(stderr, "Comparing two strings with sort keys in C failed.\n");
		return FALSE;
	}
	delete myCollator;
	return TRUE;
}

std::unique_ptr<icu::TimeZone> CreateTimezone(std::string tz) {
	UnicodeString tz_uni = UnicodeString::fromUTF8(StringPiece(tz));
	return std::unique_ptr<icu::TimeZone>(TimeZone::createTimeZone(tz_uni));
}

int main()
{

	UErrorCode success = U_ZERO_ERROR;
	// UnicodeString curTZNameEn, curTZNameFr, dateReturned;
	// UDate curDate;
	// TimeZone::getAvailableIDs();
	 int32_t stdOffset,dstOffset;

	// Create a Time Zone with America/Los_Angeles
	auto tz_us = CreateTimezone("America/Los_Angeles");
	auto tz_eu = CreateTimezone("GMT");
	if (!tz_us || !tz_eu) {
		fprintf(stderr, "EEEEk\n");
	}
	fprintf(stderr, "yay\n");
	// Create a Calendar to get current date
	Calendar* calendar = Calendar::createInstance(success);
	auto curDate = calendar->getNow();

	// Use getOffset to get the stdOffset and dstOffset for the given time
	tz_us->getOffset(curDate, false, stdOffset, dstOffset, success);
	printf("%s\n%d\n","Current Time Zone STD offset:",stdOffset/(1000*60*60));
	printf("%s\n%d\n","Current Time Zone DST offset:",dstOffset/(1000*60*60));
	printf("%s\n", "Current date/time is in daylight savings time?");
	printf("%s\n", (calendar->inDaylightTime(success))?"Yes":"No");


// 	StringEnumeration *timeZoneIds = TimeZone::createEnumeration();

	UErrorCode status = U_ZERO_ERROR;
// 	const UnicodeString *zoneId = timeZoneIds->snext(status);

// 	while (zoneId != NULL && status == U_ZERO_ERROR) {
// 	  std::string zoneIdString;
// 	  zoneId->toUTF8String(zoneIdString);

// 	  std::cout << zoneIdString << std::endl;

// 	  zoneId = timeZoneIds->snext(status);
//    }

// 	status = U_ZERO_ERROR;
// 	int32_t count;
// 	auto locales = Collator::getAvailableLocales(count);
// 	fprintf(stdout, "Available collation locales:\n");
// 	for(int32_t i = 0; i < count; i++) {
// 			fprintf(stdout, "Language: %s, Country: %s\n", locales[i].getLanguage(), locales[i].getCountry());
// 	}

	auto lt_collator = std::unique_ptr<icu::Collator>(Collator::createInstance(Locale("lt"), status));
	if (!lt_collator) {
		fprintf(stderr, "Could not make LT collator!\n");
		exit(1);
	}
	StringPiece str1("yaaa");
	StringPiece str2("paaa");
	auto unistr1 = UnicodeString::fromUTF8(str1);
	auto unistr2 = UnicodeString::fromUTF8(str2);
	auto res1 = lt_collator->compare(unistr1, unistr2);
	auto de_collator = std::unique_ptr<icu::Collator>(Collator::createInstance(Locale("de"), status));
	if (!de_collator) {
		fprintf(stderr, "Could not make LT collator!\n");
		exit(1);
	}
	auto res2 = de_collator->compare(unistr1, unistr2);
	fprintf(stdout, "\n");
	if (collateWithLocaleInCPP(Locale("en", "US"), status) != TRUE)
	{
		fprintf(stderr,
		"Collate with locale in C++ failed.\n");
	} else
	{
		fprintf(stdout, "Collate with Locale C++ example worked!!\n");
	}
	return 0;
}
