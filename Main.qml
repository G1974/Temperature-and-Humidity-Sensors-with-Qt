import QtQuick
import QtQuick.Controls

Window {
    width: 800; height: 480; visible: true; title: "Μετεωρολογικός Σταθμός"; color: "#0f172a"

    function getDayName(offset) {
        var days = ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"];
        var d = new Date();
        d.setDate(d.getDate() + offset);
        return days[d.getDay()];
    }

    Rectangle {
        id: header
        width: parent.width; height: 60; color: "#1f282a"
        Text {
            anchors.centerIn: parent
            text: "   Weather Forecast  " + Qt.formatDateTime(new Date(), "hh:mm")
            color: "#38bdf8"; font.pixelSize: 22; font.bold: true
        }
    }

    Row {
        anchors.top: header.bottom; anchors.bottom: parent.bottom; width: parent.width

        // ΕΣΩΤΕΡΙΚΟΣ ΧΩΡΟΣ
        Rectangle {
            width: parent.width / 2; height: parent.height; color: "transparent"
            Column {
                anchors.centerIn: parent; spacing: 15 // Μειώσαμε λίγο το spacing για να χωρέσουν όλα

                Text { text: "Inside Area"; color: "#94a3b8"; font.pixelSize: 18; anchors.horizontalCenter: parent; bottomPadding: 10 }

                // --- Group Θερμοκρασίας ---
                Column {
                    spacing: 5
                    anchors.horizontalCenter: parent
                    Text { text: "TEMPERATURE"; color: "#ffffff"; font.pixelSize: 16; font.bold: true; anchors.horizontalCenter: parent }
                    Rectangle {
                        width: 250; height: 90; color: "#ef4444"; radius: 15
                        Text { anchors.centerIn: parent; text: sensorHandler.indoorTemp; color: "white"; font.pixelSize: 45; font.bold: true }
                    }
                }

                // --- Group Υγρασίας ---
                Column {
                    spacing: 5
                    anchors.horizontalCenter: parent
                    Text { text: "HUMIDITY"; color: "#ffffff"; font.pixelSize: 16; font.bold: true; anchors.horizontalCenter: parent }
                    Rectangle {
                        width: 250; height: 90; color: "#3b82f6"; radius: 15
                        Text { anchors.centerIn: parent; text: sensorHandler.indoorHum; color: "white"; font.pixelSize: 45; font.bold: true }
                    }
                }
            }
        }

        // ΕΞΩΤΕΡΙΚΟΣ ΧΩΡΟΣ
        Rectangle {
            width: parent.width / 2; height: parent.height; color: "#16213e"
            Column {
                anchors.centerIn: parent; spacing: 30
                Text { text: "Outside Area"; color: "#94a3b8"; font.pixelSize: 18; anchors.horizontalCenter: parent }

                Rectangle {
                    width: 250; height: 100; color: "#46dbad"; radius: 15
                    anchors.horizontalCenter: parent
                    Row {
                        anchors.centerIn: parent; spacing: 10
                        Text { text: sensorHandler.outdoorIcon; font.pixelSize: 45 }
                        Text { text: sensorHandler.outdoorTemp; color: "white"; font.pixelSize: 45; font.bold: true }
                    }
                }

                Row {
                    spacing: 15; anchors.horizontalCenter: parent
                    ForecastDay { d: "Now"; i: sensorHandler.outdoorIcon; t: sensorHandler.outdoorTemp }
                    ForecastDay { d: getDayName(1); i: "🌤️"; t: "21°" }
                    ForecastDay { d: getDayName(2); i: "☁️"; t: "19°" }
                }
            }
        }
    }

    component ForecastDay : Rectangle {
        property string d: ""; property string i: ""; property string t: ""
        width: 100; height: 130; color: "#1e293b"; radius: 10
        Column {
            anchors.centerIn: parent; spacing: 10
            Text { text: d; color: "white"; font.pixelSize: 16; anchors.horizontalCenter: parent }
            Text { text: i; font.pixelSize: 30; anchors.horizontalCenter: parent }
            Text { text: t; color: "#38bdf8"; font.pixelSize: 20; font.bold: true; anchors.horizontalCenter: parent }
        }
    }
}
