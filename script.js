// script.js

// Get the canvas element
var ctx = document.getElementById('ppgChart').getContext('2d');
var ctxDough = document.getElementById('ppgDoughnutChart').getContext('2d');
var ctxPPG = document.getElementById('ppgChartPPG').getContext('2d');
// Initialize empty arrays to store PPG data
var timeData = [];
for(var i = 0; i <= 59; i++){
    timeData.push(i);
}
var ppgData = [];
var ppgDataPPG = [];
var count = 0;
var spo2Data = 100;
// Create the chart
var ppgChart = new Chart(ctx, {
  type: 'line',
  data: {
    labels: timeData,
    datasets: [{
      label: 'BPM Each Second',
      data: ppgData,
      fill: false,
      borderColor: 'rgb(255, 99, 132)',
      tension: 0.1,
      pointRadius: 0
    }]
  },
  options: {
      plugins: {
          title: {
            display: true,
            text: 'BPM Chart',
            font: {
              size: 20
            }
          }
        },
    scales: {
      x: {
        type: 'linear',
        position: 'bottom',
        grid: {
            display: false
        },
        min: 0, max:60
      },
      y: {
        beginAtZero: true,
        grid:{
            display: false
        }, min: 40
      }
    }
  }
});


// Define data for the doughnut chart
var data = {
  labels: ['SPO2 Percentage'],
  datasets: [{
    label: 'My Dataset',
    data: [spo2Data, 100-spo2Data],
    backgroundColor: [
      'rgba(255, 99, 132, 0.5)', 'rgba(255, 255, 255, 0.5)'
    ],
    borderColor: [
      'rgba(255, 99, 132, 1)'
    ],
    borderWidth: 1
  }]
};

// Create the doughnut chart
var doughnutChart = new Chart(ctxDough, {
  type: 'doughnut',
  data: data,
  options: {
    responsive: false,
    maintainAspectRatio: false,
    plugins: {
      title: {
        display: true,
        text: 'SPO2 Percentage',
        font: {
          size: 20
        }
      }
    }
  }
});

var ppgChartPPG = new Chart(ctxPPG, {
    type: 'line',
    data: {
        labels: timeData,
        datasets: [{
          label: 'PPG Signal',
          data: ppgDataPPG,
          fill: false,
          borderColor: 'rgb(0, 200, 255)',
          tension: 0.1,
          pointRadius: 0
        }]
    },
    options: {
      plugins: {
          title: {
            display: true,
            text: 'PPG Signal',
            font: {
              size: 20
            }
          }
        },
    scales: {
      x: {
        type: 'linear',
        position: 'bottom',
        grid: {
            display: false
        },
        min: 0, max:60
      },
      y: {
        beginAtZero: true,
        grid:{
            display: false
        }, min: 40
      }
    }
  }

})
// Function to update chart with new data
function updateChart(time, ppg, spo2, ppgPPG) {
  // Add new data to arrays
  ppgData.push(ppg);
  ppgDataPPG.push(ppgPPG)
  // Limit number of data points to display (adjust as needed)
  const maxDataPoints = 60;
  if (count >= maxDataPoints) {
     ppgChart.data.labels = [];
     ppgChart.data.datasets.forEach(dataset => {
      dataset.data = [];
     });

     ppgChartPPG.data.labels = [];
     ppgChartPPG.data.datasets.forEach(dataset => {dataset.data = [];
     });

     ppgData = [];
     ppgDataPPG = [];

     ppgChart.data.labels = timeData;
     ppgChart.data.datasets.forEach(dataset => {
      dataset.data = ppgData;
     });

     ppgChartPPG.data.labels = timeData;
     ppgChartPPG.data.datasets.forEach(dataset => {
      dataset.data = ppgDataPPG;
     });

   // Update chart
   ppgChart.update();
   ppgChartPPG.update();
  }
  // Update chart
  ppgChart.update();
  ppgChartPPG.update();
  doughnutChart.data.datasets[0].data = [spo2, 100 - spo2];
  doughnutChart.update();
}


// Example of receiving data from the sensor and updating the chart
// Replace this with actual code to receive data from your sensor
// Simulate data every second
setInterval(function() {
  var time = Date.now();
  var ppg = Math.random() * (99 - 60) + 60; // Example PPG value (replace with actual sensor data)
  var spo2 = Math.random() * (99 - 90) + 90;
  var ppgPPG = Math.random() * (40 - 90) + 90;;
  updateChart(time, ppg, spo2, ppgPPG);
  count = count + 1;
  document.getElementById('bpm-value').innerText = ppg.toFixed(0);
  document.getElementById('spo2-value').innerText = spo2.toFixed(0);
  if(count > 60){
    count = 0;
  }

}, 1000);

