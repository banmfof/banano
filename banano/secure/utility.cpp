#include <banano/secure/utility.hpp>

#include <banano/lib/interface.h>
#include <banano/node/working.hpp>

static std::vector<boost::filesystem::path> all_unique_paths;

boost::filesystem::path rai::working_path ()
{
	auto result (rai::app_path ());
	switch (rai::banano_network)
	{
		case rai::banano_networks::banano_test_network:
			result /= "RaiBlocksTest";
			break;
		case rai::banano_networks::banano_beta_network:
			result /= "BananoBeta";
			break;
		case rai::banano_networks::banano_live_network:
			result /= "Banano";
			break;
	}
	return result;
}

boost::filesystem::path rai::unique_path ()
{
	auto result (working_path () / boost::filesystem::unique_path ());
	all_unique_paths.push_back (result);
	return result;
}

std::vector<boost::filesystem::path> rai::remove_temporary_directories ()
{
	for (auto & path : all_unique_paths)
	{
		boost::system::error_code ec;
		boost::filesystem::remove_all (path, ec);
		if (ec)
		{
			std::cerr << "Could not remove temporary directory: " << ec.message () << std::endl;
		}

		// lmdb creates a -lock suffixed file for its MDB_NOSUBDIR databases
		auto lockfile = path;
		lockfile += "-lock";
		boost::filesystem::remove (lockfile, ec);
		if (ec)
		{
			std::cerr << "Could not remove temporary lock file: " << ec.message () << std::endl;
		}
	}
	return all_unique_paths;
}

void rai::open_or_create (std::fstream & stream_a, std::string const & path_a)
{
	stream_a.open (path_a, std::ios_base::in);
	if (stream_a.fail ())
	{
		stream_a.open (path_a, std::ios_base::out);
	}
	stream_a.close ();
	stream_a.open (path_a, std::ios_base::in | std::ios_base::out);
}
