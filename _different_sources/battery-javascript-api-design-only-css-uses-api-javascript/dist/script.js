const [level, fill] = document.querySelectorAll('.level, .fill');
const battery = navigator.getBattery();
const colors = {
  high: '#59ef0b',
  normal: '#d76a04',
  low: '#e70707' };


battery.then(battery => {

  const incBattery = batt => {
    const battlevel = batt > 75 ? 'high' : batt < 15 ? 'low' : 'normal';
    fill.style.setProperty('--batteryLevel', `${~~batt}%`);
    fill.style.setProperty('--batteryColor', colors[battlevel]);
    level.dataset.level = fill.style.height = `${~~batt}%`;
  };

  incBattery(battery.level * 100);

  if (battery.dischargingTime == Infinity)
  level.classList.add('power');

  if (battery.charging)
  level.classList.toggle('charge');

  battery.addEventListener('levelchange', function () {
    incBattery(this.level * 100);
  });

  battery.addEventListener('chargingchange', function () {
    level.classList.toggle('charge');
  });
});