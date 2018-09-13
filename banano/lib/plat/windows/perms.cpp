#include <assert.h>
#include <banano/lib/utility.hpp>

#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>

void rai::set_umask ()
{
	int oldMode;

	auto result (_umask_s (_S_IWRITE | _S_IREAD, &oldMode));
	assert (result == 0);
}
