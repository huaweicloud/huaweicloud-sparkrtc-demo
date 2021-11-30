import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router'; 
import { CallRoomComponent } from './call-room/call-room.component';
import { JoinRoomComponent } from './join-room/join-room.component';

const routes: Routes = [
    { path: '', component: JoinRoomComponent },
]; 

@NgModule({
  imports: [RouterModule.forRoot(routes,{useHash:true})],
  exports: [RouterModule]
})
export class AppRoutingModule { }