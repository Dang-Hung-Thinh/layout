import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  static const String esp32IP = "http://192.168.1.11";
  static const String apiToken = "123456";

  Future<void> sendLedCommand(int ledIndex, String action) async {
    try {
      var url = Uri.parse('$esp32IP/$action?token=$apiToken&led=$ledIndex');
      await http.get(url);
    } catch (e) {
      print("Lỗi gửi request: $e");
    }
  }

  Future<String> fetchStatus() async {
    try {
      var url = Uri.parse('$esp32IP/status?token=$apiToken');
      var response = await http.get(url);
      return response.body;
    } catch (e) {
      return "Không kết nối ESP32";
    }
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Điều khiển ESP32',
      home: Scaffold(
        appBar: AppBar(title: const Text('Điều khiển ESP32')),
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: List.generate(3, (i) {
              return Card(
                child: ListTile(
                  title: Text('LED $i'),
                  trailing: Row(
                    mainAxisSize: MainAxisSize.min,
                    children: [
                      IconButton(
                        icon: const Icon(Icons.flash_on, color: Colors.green),
                        onPressed: () => sendLedCommand(i, "led_on"),
                      ),
                      IconButton(
                        icon: const Icon(Icons.flash_off, color: Colors.red),
                        onPressed: () => sendLedCommand(i, "led_off"),
                      ),
                    ],
                  ),
                ),
              );
            }),
          ),
        ),
      ),
    );
  }
}
