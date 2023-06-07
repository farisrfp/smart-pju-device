import React, { useState, useEffect } from 'react';

// import Accordion from '@mui/material/Accordion';
// import AccordionSummary from '@mui/material/AccordionSummary';
// import AccordionDetails from '@mui/material/AccordionDetails';
// import Typography from '@mui/material/Typography';
// import ExpandMoreIcon from '@mui/icons-material/ExpandMore';
// import TextField from '@mui/material/TextField';
// import Button from '@mui/material/Button';

const Setting = () => {
	const [voltage, setVoltage] = useState(256);
	const [current, setCurrent] = useState(59);
	const [appEUI, setAppEUI] = useState('0000000000000000');
	const [devEUI, setDevEUI] = useState('C3E005D07ED5B370');
	const [appKey, setAppKey] = useState('44F393F26FA81504493D6699523E2251');

	return (
		<>
			<div className="py-5" />
			<div className="mx-auto h-[75%] w-[350px] rounded-2xl bg-primary text-center">
				{/* <h1 className="mb-14 pt-10 text-2xl font-bold tracking-widest text-white">SETTING</h1>
				<Accordion>
					<AccordionSummary
						expandIcon={<ExpandMoreIcon />}
						aria-controls="panel1a-content"
						id="panel1a-header">
						<Typography>Sensor</Typography>
					</AccordionSummary>
					<AccordionDetails>
						<TextField
							id="outlined-basic"
							label="Voltage Sensitivity"
							variant="outlined"
							defaultValue={voltage}
							margin="normal"
						/>
						<TextField
							id="outlined-basic"
							label="Current Sensitivity"
							variant="outlined"
							defaultValue={current}
							margin="normal"
						/>
					</AccordionDetails>
				</Accordion>
				<Accordion>
					<AccordionSummary
						expandIcon={<ExpandMoreIcon />}
						aria-controls="panel1a-content"
						id="panel1a-header">
						<Typography>LoRaWAN</Typography>
					</AccordionSummary>
					<AccordionDetails>
						<TextField
							id="outlined-basic"
							label="APP EUI"
							variant="outlined"
							type="text"
							defaultValue={appEUI}
							margin="normal"
						/>
						<TextField
							id="outlined-basic"
							label="DEV EUI"
							variant="outlined"
							type="text"
							defaultValue={devEUI}
							margin="normal"
						/>
						<TextField
							id="outlined-basic"
							label="APP KEY"
							variant="outlined"
							defaultValue={appKey}
							margin="normal"
						/>
					</AccordionDetails>
				</Accordion>

				<Button variant="contained">Save</Button> */}
			</div>
		</>
	);
};

export default Setting;
