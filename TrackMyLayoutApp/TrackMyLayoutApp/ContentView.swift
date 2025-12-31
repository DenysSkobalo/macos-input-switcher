//
//  ContentView.swift
//  TrackMyLayoutApp
//
//  Created by Denys Skobalo on 31/12/2025.
//

import SwiftUI

struct ContentView: View {
    var body: some View {
        Button("Start Agent") {
            agent_active_app_start { appName in
                guard let appName = appName else { return }
                print("Active app changed: \(String(cString: appName))")
            }
        }
    }
}
