#include "catch.hpp"

#include <stdio.h>
#include "icu-collate.hpp"
#include <vector>
#include <string>

using namespace icu;


TEST_CASE("List supported timezones", "[tz]") {
	auto timeZoneIds = std::unique_ptr<StringEnumeration>(TimeZone::createEnumeration());

	UErrorCode status = U_ZERO_ERROR;
	const UnicodeString *zoneId = timeZoneIds->snext(status);
	REQUIRE(status == U_ZERO_ERROR);

	int32_t count = 0;
	while (zoneId != NULL && status == U_ZERO_ERROR) {
		count++;
		// std::string zoneIdString;
		// zoneId->toUTF8String(zoneIdString);

		// std::cout << zoneIdString << std::endl;

		zoneId = timeZoneIds->snext(status);
	}
	REQUIRE(count > 100);
}

std::unique_ptr<icu::TimeZone> CreateTimezone(std::string tz) {
	UnicodeString tz_uni = UnicodeString::fromUTF8(StringPiece(tz));
	return std::unique_ptr<icu::TimeZone>(TimeZone::createTimeZone(tz_uni));
}

TEST_CASE("Test timezone", "[tz]") {
	UErrorCode success = U_ZERO_ERROR;

	auto tz_us = CreateTimezone("America/Los_Angeles");
	auto tz_eu = CreateTimezone("GMT");
	REQUIRE(tz_us);
	REQUIRE(tz_eu);

	Calendar* calendar = Calendar::createInstance(success);
	REQUIRE(!U_FAILURE(success));

	auto curDate = calendar->getNow();

	// Use getOffset to get the stdOffset and dstOffset for the given time
	int32_t stdOffset, dstOffset;
	tz_us->getOffset(curDate, false, stdOffset, dstOffset, success);
	REQUIRE(!U_FAILURE(success));
	printf("%s\n%d\n","US Time Zone STD offset:",stdOffset/(1000*60*60));
	printf("%s\n%d\n","US Time Zone DST offset:",dstOffset/(1000*60*60));
	printf("%s\n", "US date/time is in daylight savings time?");
	printf("%s\n", (calendar->inDaylightTime(success))?"Yes":"No");

	// something like this, might change with DST?
	REQUIRE((stdOffset/(1000*60*60))  >= -10);
	REQUIRE((stdOffset/(1000*60*60))  <= -5);

	tz_eu->getOffset(curDate, false, stdOffset, dstOffset, success);
	REQUIRE(!U_FAILURE(success));

	printf("%s\n%d\n","EU Time Zone STD offset:",stdOffset/(1000*60*60));
	printf("%s\n%d\n","EU Time Zone DST offset:",dstOffset/(1000*60*60));
	printf("%s\n", "EU date/time is in daylight savings time?");
	printf("%s\n", (calendar->inDaylightTime(success))?"Yes":"No");

	// something like this
	REQUIRE((stdOffset/(1000*60*60)) >= -1);
	REQUIRE((stdOffset/(1000*60*60))  <= 1);
}

