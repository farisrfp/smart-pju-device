import { AxiosPromise } from 'axios';

import { AXIOS } from './endpoints';

export interface Settings {
	voltage: number;
	current: number;
	appEUI: string;
	devEUI: string;
	appKey: string;
}

export function readSettings(): AxiosPromise<Settings> {
	return AXIOS.get('/Settings');
}

export function updateSettings(itfSettings: Settings): AxiosPromise<Settings> {
	return AXIOS.post('/Settings', itfSettings);
}
