#include <stdafx.h>
#include "FicsitApp.h"
#include <util/Utility.h>

#include <httplib.h>
#include <json.hpp>
#include <semver.hpp>

namespace SML {
	namespace FicsitApp {
		void checkForUpdates(std::string modLoaderVersion) {
			Utility::debug("Checking for updates...");

			// Does not validate cert!
			// TODO Find a better lib.
			httplib::SSLClient cli("api.ficsit.app", 443);

			auto res = cli.Get("/v1/sml/latest-versions");
			if (res && res->status == 200) {
				auto result = nlohmann::json::parse(res->body);
				if (result.contains("success") && result.contains("data") && (bool) result["success"]) {
					auto data = result["data"];
					if (data.contains("release") && data["release"].contains("version")) {
						auto latestVersion = semver::version(data["release"]["version"]);
						auto currentVersion = semver::version("0.0.1");
						if (latestVersion > currentVersion) {
							MessageBoxA(NULL, ("An update to SML is available!"
								"\n\nYour SML Version: " + currentVersion.to_string() +
								"\nLatest SML Version: " + latestVersion.to_string() +
								"\n\nGet the latest from ficsit.app or GitHub!"
								"\nYou can disable this message in the config file."
								"\n\nClick Ok to continue mod loading.").c_str(), "Update Available", MB_ICONINFORMATION);
						} else {
							Utility::debug("Already on latest version");
						}
					}
				}
			}
		}
	}
}