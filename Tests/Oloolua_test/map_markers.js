// requires HTML interface. uses leaflet.js


var goldIcon = new L.Icon({
  iconUrl:
    "https://raw.githubusercontent.com/pointhi/leaflet-color-markers/master/img/marker-icon-2x-gold.png",
  shadowUrl:
    "https://cdnjs.cloudflare.com/ajax/libs/leaflet/0.7.7/images/marker-shadow.png",
  iconSize: [25, 41],
  iconAnchor: [12, 41],
  popupAnchor: [1, -34],
  shadowSize: [41, 41],
});

var redIcon = new L.Icon({
  iconUrl:
    "https://raw.githubusercontent.com/pointhi/leaflet-color-markers/master/img/marker-icon-red.png",
  shadowUrl:
    "https://cdnjs.cloudflare.com/ajax/libs/leaflet/0.7.7/images/marker-shadow.png",
  iconSize: [25, 41],
  iconAnchor: [12, 41],
  popupAnchor: [1, -34],
  shadowSize: [41, 41],
});

// direction a
var dir_a = [
  [-1.3631374, 36.692356],
  [-1.3626866, 36.6920532],
  [-1.3619529, 36.6918547],
  [-1.361936, 36.691375],
  [-1.3621409, 36.6907849],
];

var dir_b = [
  [-1.3630309, 36.6932085],
  [-1.3623963, 36.6925113],
  [-1.3632845, 36.6941764],
  [-1.3636486, 36.6943463],
  [-1.3644557, 36.6948951],
  [-1.3652252, 36.6954776],
  [-1.3661375, 36.6952998],
];

// create map with attributions
const map = L.map("map").setView([-0.5, 37.5], 9);

L.tileLayer("https://tile.openstreetmap.org/{z}/{x}/{y}.png", {
  maxZoom: 19,
  attribution:
    '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>',
}).addTo(map);

L.marker([-1.3630421, 36.6930803], {icon: goldIcon})
  .addTo(map)
  .bindPopup("<b>Base Station<b>")
  .openPopup();

for (var i = 0; i < dir_a.length; i++) {
  marker = new L.marker([dir_a[i][0], dir_a[i][1]], { icon: redIcon}).addTo(map);
}

for (var i = 0; i < dir_b.length; i++) {
  marker = new L.marker([dir_b[i][0], dir_b[i][1]]).addTo(map);
}
