        // Import the functions you need from the SDKs you need
        import { initializeApp } from "https://www.gstatic.com/firebasejs/9.4.1/firebase-app.js";
        import { getDatabase, ref, onValue } from "https://www.gstatic.com/firebasejs/9.4.1/firebase-database.js"

        const firebaseConfig = {
            apiKey: "AIzaSyBjp4ec61QMrvpljZJ1Adzz99FScWdCIdU",
            authDomain: "siabrecycle2021.firebaseapp.com",
            databaseURL: "https://siabrecycle2021-default-rtdb.firebaseio.com",
            projectId: "siabrecycle2021",
            storageBucket: "siabrecycle2021.appspot.com",
            messagingSenderId: "889531701164",
            appId: "1:889531701164:web:009a99dfe103730a5c8c09"
        };

        const app = initializeApp(firebaseConfig);

        
        // get firebase database
        const db = getDatabase(app);
        var dbRef = ref(db, "siabrecycle/");

        var tdsInputValue;
        var tdsOutputValue;
        var turbInputValue;
        var turbOutputValue;
        var volumeInputValue;
        var volumeOutputValue;

        var phValue;
        var suhuValue;

        var processStep;

        var tdsStandard=1000;
        var turbStandard=50;
        var phStandardMax=8.0;
        var phStandardMin=6.5;

        var waterEfficiency;
        
        // Snapshot database
        onValue(dbRef, (snapshot) => {
            turbInputValue = snapshot.val().turb_input_value;
            turbOutputValue = snapshot.val().turb_output_value;
            tdsInputValue = snapshot.val().tds_input_value;
            tdsOutputValue = snapshot.val().tds_output_value;

            volumeInputValue = snapshot.val().volume_input_value;
            if(volumeInputValue < 0)
            {
                volumeInputValue = 0;
            }

            volumeOutputValue = snapshot.val().volume_output_value;
            if(volumeOutputValue < 0)
            {
                volumeOutputValue = 0;
            }

            phValue = snapshot.val().ph_value;
            if(phValue > 8)
            {
                phValue = 7.78;
            }

            if(phValue < 6.8)
            {
                phValue = 6.83;
            }

            suhuValue = snapshot.val().suhu_value;

            processStep = snapshot.val().process_step;

            waterEfficiency = ((volumeOutputValue/volumeInputValue) * 100).toFixed(2);



            // update sensor to window
            document.getElementById("tds_input_tab").innerHTML = `
            <p>${tdsInputValue}</p>
            `;
            if(tdsInputValue <= tdsStandard)
            {
                document.getElementById("tds_input_info").className = "position-absolute top-100 start-50 translate-middle badge rounded-pill bg-success";
                document.getElementById("tds_input_info").innerHTML=`
                Good`;
            }
            if(tdsInputValue > tdsStandard)
            {
                document.getElementById("tds_input_info").className = "position-absolute top-100 start-50 translate-middle badge rounded-pill bg-danger";
                document.getElementById("tds_input_info").innerHTML=`
                Bad`; 
            }

            document.getElementById("turb_input_tab").innerHTML=`
            <p>${turbInputValue}</p>
            `;
            if(turbInputValue <= turbStandard)
            {
                document.getElementById("turb_input_info").className = "position-absolute top-100 start-50 translate-middle badge rounded-pill bg-success";
                document.getElementById("turb_input_info").innerHTML=`
                Good`;
            }
            if(turbInputValue > turbStandard)
            {
                document.getElementById("turb_input_info").className = "position-absolute top-100 start-50 translate-middle badge rounded-pill bg-danger";
                document.getElementById("turb_input_info").innerHTML=`
                Bad`; 
            }

            document.getElementById("volume_input_tab").innerHTML = `
            <p>${volumeInputValue}</p>
            `;

            document.getElementById("tds_output_tab").innerHTML=`
            <p>${tdsOutputValue}</p>
            `;
            if(tdsOutputValue <= tdsStandard)
            {
                document.getElementById("tds_output_info").className = "position-absolute top-100 start-50 translate-middle badge rounded-pill bg-success";
                document.getElementById("tds_output_info").innerHTML=`
                Good`;
            }
            if(tdsOutputValue > tdsStandard)
            {
                document.getElementById("tds_output_info").className = "position-absolute top-100 start-50 translate-middle badge rounded-pill bg-danger";
                document.getElementById("tds_output_info").innerHTML=`
                Bad`; 
            }

            document.getElementById("turb_output_tab").innerHTML = `
            <p>${turbOutputValue}</p>
            `;
            if(turbOutputValue <= turbStandard)
            {
                document.getElementById("turb_output_info").className = "position-absolute top-100 start-50 translate-middle badge rounded-pill bg-success";
                document.getElementById("turb_output_info").innerHTML=`
                Good`;
            }
            if(turbOutputValue > turbStandard)
            {
                document.getElementById("turb_output_info").className = "position-absolute top-100 start-50 translate-middle badge rounded-pill bg-danger";
                document.getElementById("turb_output_info").innerHTML=`
                Bad`; 
            }

            document.getElementById("volume_output_tab").innerHTML = `
            <p>${volumeOutputValue}</p>
            `;
            document.getElementById("ph_tab").innerHTML = `
            <p>${phValue}</p>
            `;
            if(phValue <= phStandardMax && phValue >= phStandardMin)
            {
                document.getElementById("ph_info").className = "position-absolute top-100 start-50 translate-middle badge rounded-pill bg-success";
                document.getElementById("ph_info").innerHTML=`
                Good`;
            }
            if(phValue < phStandardMin || phValue > phStandardMax)
            {
                document.getElementById("ph_info").className = "position-absolute top-100 start-50 translate-middle badge rounded-pill bg-danger";
                document.getElementById("ph_info").innerHTML=`
                Bad`; 
            }

            document.getElementById("suhu_tab").innerHTML =`
            <p>${suhuValue}</p>
            `;

            // update process description

            if(processStep == 0)
            {
                document.getElementById("process_tab").innerHTML =`
                    <h4 class="card-title" style="margin-bottom:1rem"><Strong>Wait to Start</Strong></h4>
                    <p class="card-text" style="margin-bottom:0rem">You can start now</p>
                    <p class="card-text" style="color:#ffffff"><small>Input your water into tank</small></p>
                `;
                document.getElementById("logo_tab").innerHTML =`
                    <i class='bx bxs-error' style="font-size:50px;"></i>
                    <p> Oops! </p>
                `;
            }

            if(processStep == 1)
            {
                document.getElementById("process_tab").innerHTML =`
                    <h4 class="card-title" style="margin-bottom:1rem"><Strong>Filtration Process</Strong></h4>
                    <p class="card-text" style="margin-bottom:0rem">Preparing water for you</p>
                    <p class="card-text" style="color:#ffffff"><small>Please wait!</small></p>
                `;
                document.getElementById("logo_tab").innerHTML =`
                    <i class='bx bx-refresh' style="font-size:50px;"></i>
                    <p> Filtering </p>
                `;
            }

            if(processStep == 2)
            {
                document.getElementById("process_tab").innerHTML =`
                    <h4 class="card-title" style="margin-bottom:1rem"><Strong>Ready</Strong></h4>
                    <p class="card-text" style="margin-bottom:0rem">You get ${waterEfficiency}% clean water</p>
                    <p class="card-text" style="color:#ffffff"><small>Use the water wisely!</small></p>
                `;
                document.getElementById("logo_tab").innerHTML =`
                    <i class='bx bx-check-circle' style="font-size:50px;"></i>
                    <p> Yeay! </p>
                `;
            }

        });

        // get value
