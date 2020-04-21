#include <stdio.h>
#include "unicode/unistr.h"
#include "unicode/utypes.h"
#include "unicode/locid.h"
#include "unicode/coll.h"
#include "unicode/tblcoll.h"
#include "unicode/coleitr.h"
#include "unicode/sortkey.h"
#include "unicode/udata.h"

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

#include <string>
#include <fstream>
#include <vector>
using namespace std;

string readFile2(const string &fileName)
{
    ifstream ifs(fileName.c_str(), ios::in | ios::binary | ios::ate);

    ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, ios::beg);

    vector<char> bytes(fileSize);
    ifs.read(bytes.data(), fileSize);

    return string(bytes.data(), fileSize);
}


int main()
{
	string data = readFile2("data/icudt66l.dat");
	UErrorCode err;
	udata_setCommonData(data.c_str(), &err);

	UErrorCode status = U_ZERO_ERROR;
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
