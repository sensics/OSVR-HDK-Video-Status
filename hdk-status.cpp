/** @file
    @brief A tiny application to access the HDK's video status, and display it
   on startup and any time it changes.

    @date 2016

    @author
    Sensics, Inc.
    <http://sensics.com/osvr>
*/

// Copyright 2016 Sensics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Internal Includes
#include <osvr/ClientKit/Context.h>
#include <osvr/ClientKit/Interface.h>

// Library/third-party includes
// - none

// Standard includes
#include <iostream>
#include <chrono>
#include <thread>

/// The values that the /semantic/status/videoStatus path can take with their
/// meaning
enum class VideoStatus : int {
    Unknown = 0,
    NoInput = 1,
    Portrait = 2,
    Landscape = 3
};

static bool gotVidStatus = false;
static VideoStatus vidStatus = VideoStatus::Unknown;

void videoStatusCallback(void * /*userdata*/,
                         const OSVR_TimeValue * /*timestamp*/,
                         const OSVR_AnalogReport *report) {
    /// The state here is actually an integer, serving as an enum, so we have to
    /// (carefully) extract it.
    VideoStatus newStatus =
        static_cast<VideoStatus>(static_cast<int>(report->state));

    if (!gotVidStatus || newStatus != vidStatus) {
        /// If we haven't shown the state yet, or if the state has changed, we
        /// should display it.
        gotVidStatus = true;
        vidStatus = newStatus;

        std::cout << "\n\nOSVR HDK reports current video status: ";
        switch (vidStatus) {
        case VideoStatus::Unknown:
            std::cout << "unknown/unavailable (is your firmware up to date?)";
            break;
        case VideoStatus::NoInput:
            std::cout << "receiving no video input";
            break;
        case VideoStatus::Portrait:
            std::cout << "receiving portrait video input";
            break;
        case VideoStatus::Landscape:
            std::cout << "receiving landscape video input";
            break;
        }
        std::cout << std::endl;
    }
}

int main() {
    osvr::clientkit::ClientContext context("com.sensics.osvrhdkvideostatus");

    osvr::clientkit::Interface hdkVideoStatus = context.getInterface(
        "/com_osvr_Multiserver/OSVRHackerDevKit0/semantic/status/videoStatus");

    hdkVideoStatus.registerCallback(&videoStatusCallback, NULL);

    /// Run forever - the user can close without harm.
    while (true) {
        context.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "Library shut down, exiting." << std::endl;
    return 0;
}
