<!-- Title -->
<span align = "center">

# SIAB Recycle

### Garuda Hacks 2.0
  
</span>
<!-- End of Title -->
  
## Inspiration
With a population of 273 million people, Indonesia is the fourth most populous country in the world and claims Southeast Asia’s largest economy. However, For many households, water sources are distant, contaminated or expensive, and household sanitation is unaffordable. About 18 million Indonesians lack safe water and 20 million lack access to improved sanitation facilities. Data from WHO, 19% of the world's population has unsafe water sources. Also, 829,000 people die every year from diarrhoea due to unsafe water and poor sanitation. Based on data from Bappenas in 2018, the accessibility to drinking safe water in Indonesia is 87.75%.

## What it does
SIAB Indonesia is here to solve this problem with SIAB Recycle based on IoT. SIAB Recycle consists of Integrated hardware and software used for automatic water filtration. It aims to improve the water quality that is filtered because sometimes the filtered water is not still appropriate to the standard of water quality by Ministry of Health or KEMENKES. Therefore, SIAB Indonesia will refilter automatically if it is found pH value or TDS which are not appropriate to the standard.

## How we built it
SIAB Recycle aims to filter dirty water that changes into clean water. Dirty water is put in a tub and then it is continued by a pump to the filter. The function of this filter is to reduce turbidity level and filter particles in water. Water that has been filtered will be measured to know the turbidity level and the amount of solute (TDS) in water. If water quality that has been filtered is in accordance with the standard of clean water, and then the water is continued by the pump to the last tub. However, if water quality that has been filtered does not meet the standard, and then the water is put again in the filter for filtering until the standard of water quality is met. During the filtering process, turbidity sensor, TDS sensor, and other sensors are used to measure quality from the processed water. Then, these data of the sensors are sent to Firebase database for temporary storage. SIAB Recycle website is used to show the sensor data during SIAB Recycle device that is activated by accessing Firebase database. For the Frontend, we using Javascript, HTML, and boostrap CSS library.

## Challenges we ran into
There are some challenges that we have faced. For example how to develop the hardware in limited time. We use hardware and show the data in the web, so we need to send the sensor's data from hardware to database and then the website should able to get this data. Because we made hardware that consist of some sensors, the sensor need to calibrated.

## Accomplishments that we're proud of
We are proud of our project which can detect water quality standard and the looping process from the quality to create an automatic filtration.

## What we learned
We learned a lot from this project i.e. time management and teamwork.

## What's next for SIAB Recycle (Automatic Water Filtration)
Adding parameter sensor of monitoring water quality not only from pH value, temperature, TDS but also completing with TOC parameter, DOC, and chlorine. It aims to improve the accuracy of water quality that is consumed
