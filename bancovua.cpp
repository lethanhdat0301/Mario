#include <iostream>

#include <SDL.h>

using namespace std;

void logSDLError(ostream& os,const string &msg,bool fatal=false);

void initSDL(SDL_Window* &window,SDL_Renderer* &renderer);

void quitSDL(SDL_Window* window,SDL_Renderer* renderer);

void waitUntilKeyPressed();

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "An Implementation of Code.org Painter";




void logSDLError(ostream& os,const string &msg,bool fatal){
    os << msg << " Error:" << SDL_GetError() << endl;
    if(fatal){
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);
    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
//window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
    //SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);
//Khi chạy trong môi trường bình thường (không chạy trong máy ảo)
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC);
//Khi chạy ở máy ảo (ví dụ tại máy tính trong phòng thực hành ở trường)
//renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

void quitSDL(SDL_Window* window,SDL_Renderer* renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true)
    {
        if ( SDL_WaitEvent(&e) != 0 &&
                (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{
	//Khởi tạo là nullptr để tránh lỗi 'dangling pointer'
	SDL_Texture *texture = nullptr;
	//Nạp ảnh từ tên file (với đường dẫn)
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
	//Nếu không có lỗi, chuyển đổi về dạng texture and và trả về
	if (loadedImage != nullptr){
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		SDL_FreeSurface(loadedImage);
		//Đảm bảo việc chuyển đổi không có lỗi
		if (texture == nullptr){
			logSDLError(std::cout, "CreateTextureFromSurface");
		}
	}
	else {
		logSDLError(std::cout, "LoadBMP");
	}
	return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{
	//Thiết lập hình chữ nhật đích mà chúng ta muốn vẽ ảnh vào trong
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	//Truy vẫn texture để lấy chiều rộng và cao (vào chiều rộng và cao tương ứng của hình chữ nhật đích)
	//SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    //Đưa toàn bộ ảnh trong texture vào hình chữ nhật đích
	SDL_RenderCopy(ren, tex, NULL, &dst);
}


int main(int argc,char* argv[]){
    SDL_Window *window;
    SDL_Renderer *renderer;


    initSDL(window,renderer);
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    SDL_RenderClear(renderer);
    int width = SCREEN_HEIGHT/8;
    int height = width;

    SDL_Rect filled_rect;
    filled_rect.h=width;
    filled_rect.w=width;

    for(int row=0;row<8;row++){

        for(int col=0;col<8;col++){
                filled_rect.x= row*width;
                filled_rect.y= col*width;

            if((row+col)%2==1) {
                    SDL_SetRenderDrawColor(renderer,0,0,0,0);

                    SDL_RenderFillRect(renderer,&filled_rect);
            }
            else{
                SDL_SetRenderDrawColor(renderer,255,255,255,255);
                SDL_RenderFillRect(renderer,&filled_rect);
            }
        }
    }
    //Nạp ảnh vào một biến texture
    SDL_Texture *image = loadTexture("hau.bmp", renderer);
    //Nếu có lỗi thì giải phóng hai texture đã nạp, rồi kết thúc
    if (image == nullptr){
        SDL_DestroyTexture(image);
        quitSDL(window, renderer);
    }

    //Xoá màn hình
    SDL_RenderClear(renderer);

    for(int row=0;row<8;row++){

        for(int col=0;col<8;col++){
                filled_rect.x= row*width;
                filled_rect.y= col*width;

            if((row+col)%2==1) {
                    SDL_SetRenderDrawColor(renderer,0,0,0,0);

                    SDL_RenderFillRect(renderer,&filled_rect);
            }
            else{
                SDL_SetRenderDrawColor(renderer,255,255,255,255);
                SDL_RenderFillRect(renderer,&filled_rect);
            }
        }
    }

    //Vẽ ảnh tại vị trí và kích cỡ mới (ảnh bị co lại cho khớp)

    renderTexture(image, renderer, width, width, width, width);

    //Vẽ ảnh vào giữa màn hình, giữ nguyên kích cỡ ban đầu của ảnh
    /*int iW, iH;
    SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
    int x = SCREEN_WIDTH / 2 - iW / 2;
    int y = SCREEN_HEIGHT / 2 - iH / 2;
    renderTexture(image, renderer, x, y);*/

    // Dùng lệnh hiển thị (đưa) hình đã vẽ ra mành hình
   //Khi thông thường chạy với môi trường bình thường ở nhà
    SDL_RenderPresent(renderer);
   //Khi chạy ở máy thực hành WinXP ở trường (máy ảo)
   //SDL_UpdateWindowSurface(window);

   //Đợi 1 phím bất kỳ trước khi đóng cửa sổ
    waitUntilKeyPressed();

    //Giải phóng 2 biến texture đã dùng
    SDL_DestroyTexture(image);

    SDL_RenderPresent(renderer);


    waitUntilKeyPressed();


    quitSDL(window,renderer);
    return 0;
}
